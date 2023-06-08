#include <gtest/gtest.h>
#include <Server.hpp>
#include <TimeKeeper.hpp>
#include <future>
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
    std::promise<void> beginElection;

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

//arrange
    Raft::Server::Configuration configuration;
    configuration.instanceNumbers = {1,4,5,6,9};
    configuration.selfInstanceNumber = 6;

//act
    server.configure(configuration);

//assert
    const auto actualConfiguration = server.getConfiguration();

    EXPECT_EQ(configuration.instanceNumbers, actualConfiguration.instanceNumbers );
    EXPECT_EQ(configuration.selfInstanceNumber, actualConfiguration.selfInstanceNumber);
    
}


TEST_F(ServerTests, ElectionStartedAfterProperTimeoutInterval)
{
    //arrange
    Raft::Server::Configuration configuration;
    configuration.instanceNumbers = {1,2,3,4};
    configuration.selfInstanceNumber = 3;
    configuration.minimumTimeout = 0.1;
    configuration.maximumTimeout = 0.2;
    server.configure(configuration);

    //act
    std::future<void> electionBegun = beginElection.get_future();
    mockTimeKeeper->currentTime = 0.0999;
    EXPECT_NE(std::future_status::ready, electionBegun.wait_for(std::chrono::milliseconds(50)) );

    mockTimeKeeper->currentTime = 0.2;
    EXPECT_EQ(std::future_status::ready, electionBegun.wait_for(std::chrono::milliseconds(50)) );


}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


