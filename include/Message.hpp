#pragma once
#include <memory>
#include <functional>

namespace Raft {
    class Message {
        public:
            Message();
            ~Message() noexcept;
            Message(const Message&) = delete;
            Message(Message&&) noexcept;
            Message& operator=(const Message&) = delete;
            Message& operator=(Message&&) noexcept;

            //Public class properties
        public:
            static std::function<std::shared_ptr<Message>()> createMessage; //factory function called by msg class to create new msg internally
        
        //package-private properties (public but opaque)
        public:
            std::unique_ptr<struct MessageImpl> impl_; 

    };

}