#ifndef RAFT_TIME_KEEPER_HPP
#define RAFT_TIME_KEEPER_HPP

namespace Raft {
    class TimeKeeper {
        public:    
            virtual double getCurrentTime() = 0;
    };
}

#endif 