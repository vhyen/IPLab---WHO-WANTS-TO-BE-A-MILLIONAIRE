// #include <stdio.h>
// #include <string>
// #include <iostream>
// #include <sys/socket.h>
// #include "Play.hpp"
// int main()
// {
//     int sockfd;//get from client.cpp
//     Play newGame;
//     newGame.Init();
//     //Interaction, user choose to join and set up username...
//     newGame.Registered();
//     //if receive message is 'T', this is client turn to play
//     char*rcvMessage=new char[1];
//     //display waiting screen
//     while(1)
//     {
//     recv(sockfd, rcvMessage, 1, 0);
//     while(rcvMessage[0]=='T')
//     {
//         char *rcvQuestion=new char[100]; //may change this constant
//         recv(sockfd, rcvQuestion, 1, 0);
//         //Seprate receive qestion into 5 string quest, óp, op2, op3, op4.
//         std::string ques, op1, op2, op3, op4;
//         newGame.PlayATurn(ques, op1, op2, op3, op4);
//     }
//     }
//     return 0;
// }