#pragma once
#include <vector>
namespace Raft {
    class IServer {
    public:
        struct Configuration {
            std::vector<unsigned int> instanceNumbers;
            unsigned int selfInstanceNumber = 0;
        };

        virtual bool configure(const Configuration&) = 0;

    };
}