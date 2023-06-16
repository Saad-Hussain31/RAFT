#include "Message.hpp"


namespace Raft {
    struct Message::Impl {
        bool isElectionMessage = false;
    };
    
    Message::~Message() noexcept = default;
    Message::Message(Message&&) noexcept = default;
    Message& Message::operator=(Message&&) noexcept = default;

    Message::Message()
        : impl_(new Impl())
    {
    }

    bool Message::isElectionMessage() const {
        return impl_->isElectionMessage;
    }

    void Message::formElectinMessage() {
        impl_->isElectionMessage = true;
    }


} 
