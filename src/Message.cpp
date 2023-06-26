#include "Message.hpp"
#include "MessageImpl.hpp"


namespace {
    std::shared_ptr<Raft::Message> createBaseMessage() {
        return std::make_shared<Raft::Message>();
    }
}

namespace Raft {
    
    Message::~Message() noexcept = default;
    Message::Message(Message&&) noexcept = default;
    Message& Message::operator=(Message&&) noexcept = default;

    std::function<std::shared_ptr<Message>()> Message::createMessage = createBaseMessage;

    Message::Message()
        : impl_(new MessageImpl())
    {
    }

} 
