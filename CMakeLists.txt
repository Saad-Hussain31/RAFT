cmake_minimum_required(VERSION 3.20)
project(RAFT)
set(CMAKE_CXX_STANDARD 17)


add_subdirectory(modules/googletest)
add_subdirectory(test)

include_directories(include)
include_directories(3rdparty/header/)


set(Headers
    include/IServer.hpp
    include/Server.hpp
    include/LogEntry.hpp
    include/TimeKeeper.hpp
    include/Message.hpp
)

set(Sources
    src/Server.cpp
    src/Message.cpp
)



add_library(Raft STATIC ${Sources} ${Headers})
# target_include_directories(Raft PUBLIC include)


target_link_libraries(Raft PUBLIC
                    /home/alma/raft/3rdparty/lib/libStringExtensions.a
                    /home/alma/raft/3rdparty/lib/libSystemAbstractions.a #for maknig things cross platform
                    )




