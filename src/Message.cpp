#include "include/Message.hpp"

namespace {
    struct ServerSharedProperties {
        SystemAbstractions::DiagnosticsSender diagnosticsSender;
        Raft::IServer::Configuration configuration;
        ServerSharedProperties() : diagnosticsSender("Raft::Server") {}

    };
}


namespace Raft {
    struct Server::Impl {
        std::shared_ptr< ServerSharedProperties > shared = std::make_shared< ServerSharedProperties >();
        std::shared_ptr< TimeKeeper > timeKeeper;
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

    auto Server::getConfiguration() const -> const Configuration& {
        return impl_->shared->configuration;
    }


} 
