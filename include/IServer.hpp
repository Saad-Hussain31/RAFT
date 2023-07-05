#pragma once
#include <vector>
#include <functional>
#include <memory>
#include "Message.hpp"

namespace Raft {
    class IServer {
        public:
            struct Configuration {
                std::vector<unsigned int> instanceNumbers;
                unsigned int selfInstanceNumber = 0;
                unsigned int currentTerm = 0; //last term the server has seen. 
                double minimumTimeout = 0.15;
                double maximumTimeout = 0.3;
            };

            virtual bool configure(const Configuration&) = 0;

            using SendMessageDelegate = std::function<void(std::shared_ptr<Message> message)>;
            virtual void setSendMessageDelegate(SendMessageDelegate setSendMessageDelegate) = 0;
            virtual void receiveMessage(std::shared_ptr<Message> message, unsigned int senderInstanceNumber) = 0;
            virtual bool isLeader() = 0;
    };
}