#ifndef Play_hpp
#define Play_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <sys/socket.h>
#include <SFML/Graphics.hpp>
#include "SharedResource.hpp"
#include "Player.hpp"

class Play {
private:
    std::string username;
    bool stillQualified;
    bool isMyTurn;
    bool join;
    int numTurn;
    bool skip; //determined if user has chosen to skip 1 question yet. False: no skip yet. True: already skip.
    int sockfd; //socket to send and receive message

    sf::RenderWindow* window;
    std::vector<Player> players;
    std::vector<std::string> usernames;
    std::string curPlayer;

public:
    Play();
    void setWindow(sf::RenderWindow* wd);
    void Init(); //inital status of user
    void Registered(); //is call when user want to create a username
    void Skip();//is Call when user choose to skip a question 
    void PlayATurn(std::string question, std::string optionA, std::string optionB, std::string optionC, std::string optionD);//is call when it is user's turn and server send a question
    
    // init resources
    void initPlayers(std::vector<std::string>);

    // render
    void renderPlayers();
    void renderQuestion();
    void render();
};

#endif 