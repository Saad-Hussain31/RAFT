#include "TimeKeeper.hpp"
#include "Server.hpp"
#include <SystemAbstractions/DiagnosticsSender.hpp>
#include <thread>
#include <future>

namespace {
    const std::chrono::milliseconds WORKER_POLLING_PERIOD = std::chrono::milliseconds(50);

    struct ServerSharedProperties {
        SystemAbstractions::DiagnosticsSender diagnosticsSender;
        Raft::IServer::Configuration configuration;
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
        void Worker() {
            double timeOfLastLeaderMessage = timeKeeper->getCurrentTime();
            auto workerAskedToStop = stopWorker.get_future();
            while(workerAskedToStop.wait_for(WORKER_POLLING_PERIOD) != std::future_status::ready) {
                const auto now  = timeKeeper->getCurrentTime();
                const auto timeSinceLastLeaderMessage = now - timeOfLastLeaderMessage;
                if(timeSinceLastLeaderMessage >= shared->configuration.minimumTimeout) {
                    const auto message = std::make_shared<Message>();
                    message->formElectinMessage(); 
                    sendMessageDelegate(message);
                }
            }
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

    void Server::setTimeKeeper(std::shared_ptr< TimeKeeper > timeKeeper) {
        impl_->timeKeeper = timeKeeper;
    }

    bool Server::configure(const Configuration& configuration) {
        impl_->shared->configuration = configuration;
        return true;
    }

    void  Server::setSendMessageDelegate(SendMessageDelegate sendMessageDelegate) {
        impl_-> sendMessageDelegate = sendMessageDelegate;
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

} 
