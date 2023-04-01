#include "Play.hpp"
void Play::Init()
{
    this->skip=false;
    this->join=false;
    this->stillQualified=false;
}
void Play::Registered()
{
    std::cout<<"Choose a nickname to play. Only letters, digits and underscore allowed!\n";
    std::string name;
    std::cin>>name;
    //Send name to server. Server checks if the name is ok. Server returns message.
    //Display message from server to screen
    //If ok:
    this->username=name;
    this->join=true;
    this->stillQualified=true;
    //If not: Do nothing
}
void Play::Skip()
{
    std::cout<<"You have chosen to skip this question.\n";
    this->skip=true;
}
void Play::PlayATurn(std::string question, std::string optionA, std::string optionB, std::string optionC, std::string optionD)
{
    char choice;
    std::cout<<"Questions: "<<question<<std::endl;
    std::cout<<"Option A: "<<optionA<<std::endl;
    std::cout<<"Option B: "<<optionB<<std::endl;
    std::cout<<"Option C: "<<optionC<<std::endl;
    std::cout<<"Option D: "<<optionD<<std::endl;
    if (this->skip==false)
    {
    char skip;
    std::cout<<"Do you want to skip this question? Remember you are only allowed to skip once!"<<std::endl;
    std::cout<<"Enter N: No. Enter Y: yes."<<std::endl;
    std::cin>>skip;
    if(skip=='Y')
    {
        Skip();
        //Send message to server to forward the question
        return;
    }
    }
    std::cout<<"Your Choice (enter a letter A or a, B or b, C or c, D or d): "<<std::endl;
    std::cin>>choice;
    while (choice!='a'&&choice!='b'&&choice!='c'&&choice!='d'&&choice!='A'&&choice!='B'&&choice!='C'&&choice!='D')
    {
        std::cout<<"Invalid choice. Choose again."<<std::endl;
        std::cin>>choice;
    }
    //Send choice to server, server checks if this user stil qualified, or winner, ...
    //Receive message from server.
    //If not qualified anymore:
    //this->stillQualified=false;
    //If winner
    //Display congratualation message!
    //If still qualified
    //go back to waiting status. Displaying waiting GUI
}
