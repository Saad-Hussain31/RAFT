//this mod contains the decleration of package-private struct


namespace Raft {
    struct MessageImpl {

        //types of messages
        enum class Type {
            Unknown,
            RequestVote,
            RequestVoteResults,
        };
        
        Type type = Type::Unknown;
        
        struct RequestVoteDetails{ 
            unsigned int candidateId = 0; //instanceID of candidate requesting vote
            unsigned int term = 0;
        };

         struct RequestVoteResultDetails{ 
            unsigned int term = 0; 
            unsigned int voteGranted = 0;
        };
        
        union { //holds properties specific to each type of message
            RequestVoteDetails requestVote;
            RequestVoteResultDetails requstVoteResults;
        };
    
        MessageImpl();
    };
} 
