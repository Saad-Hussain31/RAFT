#include "TimeKeeper.hpp"
#include "Server.hpp"
#include <SystemAbstractions/DiagnosticsSender.hpp>
#include <thread>
#include <future>
#include <mutex>

namespace {
    const std::chrono::milliseconds WORKER_POLLING_PERIOD = std::chrono::milliseconds(50);

    struct ServerSharedProperties {
        SystemAbstractions::DiagnosticsSender diagnosticsSender;
        Raft::IServer::Configuration configuration;
        std::mutex mutex;
        std::shared_ptr<std::promise<void>> workerLoopCompletion;
        ServerSharedProperties() : diagnosticsSender("Raft::Server") {}

    };
}


namespace Raft {
    struct Server::Impl {
        //Properties
        std::shared_ptr< ServerSharedProperties > shared = std::make_shared< ServerSharedProperties >();
        std::shared_ptr< TimeKeeper > timeKeeper;
        SendMessageDelegate sendMessageDelegate;
        std::thread worker;
        std::promise<void> stopWorker;

        bool makeWorkerThreadLoopPromiseIfNeeded() {
            std::lock_guard<decltype(shared->mutex)> lock(shared->mutex);
            return(shared->workerLoopCompletion != nullptr);
        }


        /*
        * Runs on the worker thread and check for messages from the leader. 
        * If no message is received within a specified timeout, it starts a new election.
        */
        void Worker() {
            shared->diagnosticsSender.SendDiagnosticInformationString(0, "Worker thread started"); //this is just a log with min severity
            double timeOfLastLeaderMessage = timeKeeper->getCurrentTime(); //last time a message was received from the leader
            auto workerAskedToStop = stopWorker.get_future(); //check if the worker has been asked to stop

            //while the worker has not been asked to stop
            while(workerAskedToStop.wait_for(WORKER_POLLING_PERIOD) != std::future_status::ready) {
                const auto singleWorkerLoopCompletion = makeWorkerThreadLoopPromiseIfNeeded();
                const auto now  = timeKeeper->getCurrentTime();
                const auto timeSinceLastLeaderMessage = now - timeOfLastLeaderMessage;
                if(timeSinceLastLeaderMessage >= shared->configuration.minimumTimeout) {
                    const auto message = std::make_shared<Message>();
                    message->formElectinMessage(); 
                    shared->diagnosticsSender.SendDiagnosticInformationString(1, "Timeout -- Starting new Election.");
                    sendMessageDelegate(message);
                }
                if(singleWorkerLoopCompletion) {
                    shared->workerLoopCompletion->set_value();
                    shared->workerLoopCompletion = nullptr;
                }
            }
            if(shared->workerLoopCompletion != nullptr) {
                shared->workerLoopCompletion->set_value();
                shared->workerLoopCompletion = nullptr;
            }
            shared->diagnosticsSender.SendDiagnosticInformationString(0, "Worker Thread stopping.");
        }
    };
    
    Server::~Server() noexcept = default;
    Server::Server(Server&&) noexcept = default;
    Server& Server::operator=(Server&&) noexcept = default;

    Server::Server()
        : impl_(new Impl())
    {
    }

    SystemAbstractions::DiagnosticsSender::UnsubscribeDelegate Server::SubscribeToDiagnostics(
        SystemAbstractions::DiagnosticsSender::DiagnosticMessageDelegate delegate,
        size_t minLevel
    ) {
        return impl_->shared->diagnosticsSender.SubscribeToDiagnostics(delegate, minLevel);
    }

    bool Server::configure(const Configuration& configuration) {
        impl_->shared->configuration = configuration;
        return true;
    }

    void  Server::setSendMessageDelegate(SendMessageDelegate sendMessageDelegate) {
        impl_-> sendMessageDelegate = sendMessageDelegate;
    }

    void Server::setTimeKeeper(std::shared_ptr< TimeKeeper > timeKeeper) {
        impl_->timeKeeper = timeKeeper;
    }

    auto Server::getConfiguration() const -> const Configuration& {
        return impl_->shared->configuration;
    }

    void Server::mobilize() {
        impl_->worker = std::thread(&Impl::Worker, impl_.get());
    }

    void Server::demobilize() {
        if(!impl_->worker.joinable())
            return;
        impl_->stopWorker.set_value();
        impl_->worker.join();
    }

    void Server::waitForAtleastOneWorkerLoop() {
        std::unique_lock<decltype(impl_->shared->mutex)> lock(impl_->shared->mutex);
        impl_->shared->workerLoopCompletion = std::make_shared<std::promise<void>>();
        auto workerLoopWasCompleted = impl_->shared->workerLoopCompletion->get_future();
        lock.unlock();
        workerLoopWasCompleted.wait();

    }

} 
