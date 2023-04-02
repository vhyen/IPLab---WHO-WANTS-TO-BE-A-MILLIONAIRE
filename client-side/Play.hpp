#ifndef Play_hpp
#define Play_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <sys/socket.h>


class Play {
private:
    std::string username;
    bool stillQualified;
    bool isMyTurn;
    bool join;
    int numTurn;
    bool skip; //determined if user has chosen to skip 1 question yet. False: no skip yet. True: already skip.
    int sockfd; //socket to send and receive message
public:
    Play();
    void Init(); //inital status of user
    void Registered(); //is call when user want to create a username
    void Skip();//is Call when user choose to skip a question 
    void PlayATurn(std::string question, std::string optionA, std::string optionB, std::string optionC, std::string optionD);//is call when it is user's turn and server send a question
};
#endif