#include <string>


struct Player {
    public: 
    std::string username;
    bool isQualified; 
    bool haveMove;
    
    Player();
    Player(std::string username);
}

