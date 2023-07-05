#include <gtest/gtest.h>
#include "Server.hpp"
#include "Message.hpp"
#include <TimeKeeper.hpp>
#include <future>
#include <condition_variable>
#include <mutex>
#include <vector>
#include "StringExtensions/StringExtensions.hpp"
#include "src/MessageImpl.hpp"





namespace {

    struct MockTimeKeeper : public Raft::TimeKeeper {
    
        double currentTime = 0.0;

        virtual double getCurrentTime() override {
            return currentTime;
        }
    };

}


struct ServerTests : public ::testing::Test {

    Raft::Server server;
    std::vector< std::string > diagnosticMessages;
    SystemAbstractions::DiagnosticsSender::UnsubscribeDelegate diagnosticsUnsubscribeDelegate;
    const std::shared_ptr< MockTimeKeeper > mockTimeKeeper = std::make_shared< MockTimeKeeper >();
    std::vector<std::shared_ptr<Raft::Message>> messagesSent;
    std::mutex messagesSentMutex;
    std::condition_variable messagesSentCondition;

    bool awaitServerMessagesToBeSent(size_t numMessages) {
        std::unique_lock<decltype(messagesSentMutex)> lock(messagesSentMutex);
        return messagesSentCondition.wait_for(lock,
               std::chrono::milliseconds(1000), 
               [this, numMessages] {return messagesSent.size() >= numMessages; });
    }

    void serverSentMessage(std::shared_ptr<Raft::Message> message) {

        std::lock_guard<decltype(messagesSentMutex)> lock(messagesSentMutex);
        messagesSent.push_back(message);
        messagesSentCondition.notify_one();
            
    }
    

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
        server.setSendMessageDelegate(
            [this](std::shared_ptr<Raft::Message> message) {
               serverSentMessage(message); 
            }
        );
    }

    virtual void TearDown() {
        server.demobilize();
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
    //arrange: mocks and objects would be created
    Raft::Server::Configuration configuration;
    configuration.instanceNumbers = {1,2,3,4};
    configuration.selfInstanceNumber = 3;
    configuration.minimumTimeout = 0.1;
    configuration.maximumTimeout = 0.2;
    server.configure(configuration);

    //act: the invocation of the method being tested
    server.mobilize();
   //auto electionBegan = beginElection.get_future();
    mockTimeKeeper->currentTime = 0.0999;
    server.waitForAtleastOneWorkerLoop();

    EXPECT_EQ(0, messagesSent.size()); //new 
   
    //EXPECT_NE(std::future_status::ready, electionBegan.wait_for(std::chrono::milliseconds(100)) );

    mockTimeKeeper->currentTime = 0.2;

    EXPECT_TRUE(awaitServerMessagesToBeSent(4));
   // EXPECT_EQ(std::future_status::ready, electionBegan.wait_for(std::chrono::milliseconds(100)) );

    //assert
    for(const auto message : messagesSent) {
        EXPECT_EQ(Raft::MessageImpl::Type::RequestVote, message->impl_->type);
        EXPECT_EQ(5, message->impl_->requestVote.candidateId);
    }

}

TEST_F(ServerTests, ServerVotesForItselfInElectionItStarts)
{
    //arrange: mocks and objects would be created
    Raft::Server::Configuration configuration;
    configuration.instanceNumbers = {1,2,3,4,5};
    configuration.selfInstanceNumber = 5;
    configuration.minimumTimeout = 0.1;
    configuration.maximumTimeout = 0.2;
    server.configure(configuration);
    server.mobilize();
    server.waitForAtleastOneWorkerLoop();

    //act: the invocation of the method being tested
    
   //auto electionBegan = beginElection.get_future();
    mockTimeKeeper->currentTime = 0.2;
    (void)awaitServerMessagesToBeSent(4);
    // const auto message = electionBegan.get();
    

    for(const auto message : messagesSent) {
      //  EXPECT_EQ(Raft::MessageImpl::Type::RequestVote, message->impl_->type);
        EXPECT_EQ(5, message->impl_->requestVote.candidateId);
    }
    
    
}

TEST_F(ServerTests, ServerIncrementsTermsInElectionItStarts)
{
    //arrange: mocks and objects would be created
    Raft::Server::Configuration configuration;
    configuration.instanceNumbers = {1,2,3,4,5};
    configuration.selfInstanceNumber = 5;
    configuration.minimumTimeout = 0.1;
    configuration.maximumTimeout = 0.2;
    server.configure(configuration);
    server.mobilize();
    server.waitForAtleastOneWorkerLoop();

    //act: the invocation of the method being tested
    // auto electionBegan = beginElection.get_future();
    
    mockTimeKeeper->currentTime = 0.2;
    // const auto message = electionBegan.get();
    (void)awaitServerMessagesToBeSent(4);

    for(const auto message : messagesSent) {
        EXPECT_EQ(1, message->impl_->requestVote.term);
    }
    
}


TEST_F (ServerTests, ServerDoesReceiveUnanimousVoteInElection) {
    //arrange
    Raft::Server::Configuration configuration;
    configuration.instanceNumbers = {1,2,3,4,5};
    configuration.selfInstanceNumber = 5;
    configuration.minimumTimeout = 0.1;
    configuration.maximumTimeout = 0.2;
    server.configure(configuration);
    server.mobilize();
    server.waitForAtleastOneWorkerLoop();
    // auto electionBegan = beginElection.get_future();
    mockTimeKeeper->currentTime = 0.2;
    (void)awaitServerMessagesToBeSent(4);
    // (void)electionBegan.get();

    //act
    
    for(auto instance : configuration.instanceNumbers) {
        const auto message = Raft::Message::createMessage();
        message->impl_->type = Raft::MessageImpl::Type::RequestVoteResults;
        message->impl_->requstVoteResults.term = 0;
        message->impl_->requstVoteResults.voteGranted = true;
        server.receiveMessage(message, instance);
    }

    //ASSERT:
    EXPECT_TRUE(server.isLeader());
   
}



TEST_F (ServerTests, ServerDoesReceiveNonUnanimousMajorityVoteInElection) {
    //arrange
    Raft::Server::Configuration configuration;
    configuration.instanceNumbers = {1,2,3,4,5};
    configuration.selfInstanceNumber = 5;
    configuration.minimumTimeout = 0.1;
    configuration.maximumTimeout = 0.2;
    server.configure(configuration);
    server.mobilize();
    server.waitForAtleastOneWorkerLoop();
    // auto electionBegan = beginElection.get_future();
    mockTimeKeeper->currentTime = 0.2;
    (void)awaitServerMessagesToBeSent(4);
    // (void)electionBegan.get();

    //act
    
    for(auto instance : configuration.instanceNumbers) {
        const auto message = Raft::Message::createMessage();
        message->impl_->type = Raft::MessageImpl::Type::RequestVoteResults;
        
        if(instance == 11) {
            message->impl_->requstVoteResults.term = 1;
            message->impl_->requstVoteResults.voteGranted = false;
        } else {
            message->impl_->requstVoteResults.term =0;
            message->impl_->requstVoteResults.voteGranted = true;
        }
        
        server.receiveMessage(message, instance);
    }

    //ASSERT:
    EXPECT_TRUE(server.isLeader());
   
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


