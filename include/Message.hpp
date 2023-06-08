#pragma once
#include <memory>


namespace Raft {
    class Message {
        public:
            Message();
            ~Message() noexcept;
            Message(const Message&) = delete;
            Message(Message&&) noexcept;
            Message& operator=(const Message&) = delete;
            Message& operator=(Message&&) noexcept;


            bool isElectionMessage() const;
        
        private:
            struct Impl;
            std::unique_ptr<Impl> impl_;

    };

}