#include "Play.hpp"
Play::Play()
{
    //Display entering screen
}
void Play::Init()
{
    this->skip=false;
    this->join=false;
    this->stillQualified=false;
    //Display initial screen;
}
void Play::Registered()
{
    std::cout<<"Choose a nickname to play. Only letters, digits and underscore allowed!\n";
    std::string name;
    std::cin>>name;
    bool check;
    if (name.length()>10)
    {
        std::cout<<"Username is too long. Choose another\n";
        Registered();
        return;
    }
    for (int j=0; j<name.length(); ++j)
    {
        check=((name[j] >= 'a') && (name[j] <= 'z')) || 
        ((name[j] >= 'A') && (name[j] <= 'Z')) || ((name[j] >= '0') && (name[j] <= '9')) || (name[j]=='_');
        if (check==false)
        {
            std::cout<<"Disqualified username. Choose another (consists of upper/lower case letters and/or digits and/or underscore\n";
            Registered();
            return;
        }
    }
    char*nameSend=new char[name.length()];
    char*nameMessage=new char[1];
    for(int i=0; i<name.length(); ++i)
    {
        nameSend[i]=name[i];
    }
    send(sockfd, nameSend, name.length(), 0);
    //receive message from the server to see if name registered is OK...  0: Already registered name. 1: Success 
    recv(sockfd, nameMessage, 1, 0);
    if (nameMessage[0]==0)
    {
        std::cout<<"Already registered username. Choose another\n";
        Registered();
        return;
    }
    else if (nameMessage[0]==1)
    {
        std::cout<<"Successfully assigned username.\n";
        this->username=name;
        this->join=true;
        this->stillQualified=true;
        return;
    }
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
        //Send message to server to forward the question. Send char S.
        char *skipChoice=new char[1];
        char *skipApprove=new char[1];
        skipChoice[0]='S';
        send(sockfd, skipChoice, 1, 0);
        Skip();
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
    char *choiceSend=new char[1];
    char *choiceRcv=new char[1];
    choiceSend[0]=choice;
    send(sockfd, choiceSend, 1, 0);
    //Receive message from server.
    //0: out of game, 1: in game, 2: winner.
    recv(sockfd, choiceRcv, 1, 0);
    if (choiceRcv[0]==0)
    {
        this->stillQualified=false;
    }
    else if(choiceRcv[0]==1)
    {
        //go back to waiting status. Displaying waiting GUI
    }
    else if (choiceRcv[0]==2)
    {
        //Display congratualation message!
    }
    else
    {
       //Display error message!
    }

}
