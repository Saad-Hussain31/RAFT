#pragma once
#include <vector>
#include <functional>
#include <memory>

namespace Raft {
    class IServer {
        public:
            struct Configuration {
                std::vector<unsigned int> instanceNumbers;
                unsigned int selfInstanceNumber = 0;
                double minimumTimeout = 0.15;
                double maximumTimeout = 0.3;
            };

            virtual bool configure(const Configuration&) = 0;

            virtual void setSendMessageDelegate(setSendMessageDelegate setSendMessageDelegate) = 0;
            using SendMessageDelegate = std::function<std::shared_ptr<Message> message>;
    };
}