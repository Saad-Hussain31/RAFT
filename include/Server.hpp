#pragma once 

#include <memory>
#include "IServer.hpp"
#include "SystemAbstractions/DiagnosticsSender.hpp"
#include "TimeKeeper.hpp"

namespace Raft {
    class Server : public IServer {
        public:
            Server();
            ~Server() noexcept;
            Server(const Server&) = delete; //copy ctr
            Server(Server&&) noexcept; //move ctor
            Server& operator=(const Server&) = delete; //copy asg operator
            Server& operator=(Server&&) noexcept; //move asg operator
            
            virtual bool configure(const Configuration& configuration) override;
            virtual void  setSendMessageDelegate(SendMessageDelegate sendMessageDelegate) override;

            void setTimeKeeper(std::shared_ptr<TimeKeeper> timeKeeper);
            const Configuration& getConfiguration() const;
            void mobilize(); //starts the server's worker thd
            void demobilize(); //stops th e server's worker thd
            void waitForAtleastOneWorkerLoop(); //blocks until server's worker thd executes at least 1 more loop
            


            SystemAbstractions::DiagnosticsSender::UnsubscribeDelegate SubscribeToDiagnostics(
                SystemAbstractions::DiagnosticsSender::DiagnosticMessageDelegate delegate,
                size_t minLevel = 0
            );
            
        private:
            struct Impl;
            std::unique_ptr<Impl> impl_;
    };
}