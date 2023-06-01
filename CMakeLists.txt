cmake_minimum_required(VERSION 3.20)
project(RAFT)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(modules/googletest)
add_subdirectory(modules/SystemAbstractions)



include_directories(include)

set(Headers
    include/IServer.hpp
    include/Server.hpp
    include/LogEntry.hpp
    include/TimeKeeper.hpp
)

set(Sources
    src/Server.cpp
)
 
add_library(Raft STATIC  ${Sources} ${Headers})
target_include_directories(Raft PUBLIC include)
target_link_libraries(Raft SystemAbstractions)




