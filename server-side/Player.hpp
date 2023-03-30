#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include <string>

class Player {
    public:
    std::string username;
    bool isQualified;
    bool haveMove;

    Player();
    Player(std::string username);
};



#endif /* Player_hpp */