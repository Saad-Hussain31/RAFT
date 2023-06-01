cmake_minimum_required(VERSION 3.8)


set(Sources
    src/ServerTests.cpp
)

add_executable(RaftTests ${Sources})

target_link_libraries(${This} PUBLIC
    gtest
    Raft
    SystemAbstractions
)

add_test(
    NAME ${This}
    COMMAND ${This}
)
