//this mod contains the decleration of package-private struct


namespace Raft {
    struct MessageImpl {

        //types of messages
        enum class Type {
            Unknown,
            Election,
        };
        
        Type type = Type::Unknown;
        
        struct ElectionDetails{ //holds message properties for Type::Election msgs
            unsigned int candidateId = 0; //instanceID of candidate requesting vote
        };
        
        union { //holds properties specific to each type of message
            ElectionDetails election;
        };
    
        MessageImpl();
    };
} 
