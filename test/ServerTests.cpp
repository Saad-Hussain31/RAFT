#include <gtest/gtest.h>
#include <Server.hpp>
#include <TimeKeeper.hpp>
#include "StringExtensions/StringExtensions.hpp"





namespace {

    struct MockTimeKeeper : public Raft::TimeKeeper {
    
        double currentTime = 0.0;

        virtual double getCurrentTime() override {
            return currentTime;
        }
    };

}


struct ServerTests
    : public ::testing::Test {

    Raft::Server server;
    std::vector< std::string > diagnosticMessages;
    SystemAbstractions::DiagnosticsSender::UnsubscribeDelegate diagnosticsUnsubscribeDelegate;
    const std::shared_ptr< MockTimeKeeper > mockTimeKeeper = std::make_shared< MockTimeKeeper >();


    virtual void SetUp() {
        diagnosticsUnsubscribeDelegate = server.SubscribeToDiagnostics(
            [this](
                std::string senderName,
                size_t level,
                std::string message
            ){
                diagnosticMessages.push_back(
                    StringExtensions::sprintf(
                        "%s[%zu]: %s",
                        senderName.c_str(),
                        level,
                        message.c_str()
                    )
                );
            },
            0
        );
        server.setTimeKeeper(mockTimeKeeper);
    }

    virtual void TearDown() {
        diagnosticsUnsubscribeDelegate();
    }
};

TEST_F(ServerTests, InitialConfiguration) 
{
    Raft::Server::Configuration configuration;
    configuration.instanceNumbers = {1,4,5,6,9};
    configuration.selfInstanceNumber = 6;

    server.configure(configuration);

    const auto actualConfiguration = server.getConfiguration();

    EXPECT_EQ(configuration.instanceNumbers, actualConfiguration.instanceNumbers );
    EXPECT_EQ(configuration.selfInstanceNumber, actualConfiguration.selfInstanceNumber);
    
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}