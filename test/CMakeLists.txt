cmake_minimum_required(VERSION 3.8)


include_directories(../3rdparty/header/)
include_directories(../include)



set(Sources ServerTests.cpp)

add_executable(RaftTests ${Sources})



target_link_libraries(RaftTests PUBLIC
    gtest
    # Raft
    /home/alma/raft/build/libRaft.a
    /home/alma/raft/3rdparty/lib/libStringExtensions.a
    /home/alma/raft/3rdparty/lib/libSystemAbstractions.a
)

