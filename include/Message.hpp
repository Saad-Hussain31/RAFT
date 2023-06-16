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


            bool isElectionMessage() const; //flag that tells whether a message is an election message
            void formElectinMessage(); //forms the message to be a election message
        
        private:
            struct Impl;
            std::unique_ptr<Impl> impl_;

    };

}