#pragma once
#include <vector>
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

    };
}