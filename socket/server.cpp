#include <stdio.h> 
#include <string.h>  
#include <stdlib.h> 
#include <unistd.h>   
#include <sys/socket.h> 
#include <netinet/in.h> 

#define PORT 8888 
     
int main(int argc , char *argv[])  
{  
    int listening = socket(AF_INET , SOCK_STREAM , 0);
    sockaddr_in address;
    address.sin_family = AF_INET;  
    address.sin_port = htons( PORT );  
    address.sin_addr.s_addr = INADDR_ANY; 
    if (listening < 0) 
    {  
        perror("Socket Creation Failed");  
        exit(EXIT_FAILURE);  
    }  
    int binding =bind(listening, (sockaddr*)& address, sizeof(address));
    if (binding < 0)
    {
        perror("Binding Establishment Failed");  
        exit(EXIT_FAILURE);  
    }
         
    listen (listening, SOMAXCONN);
    fd_set socketSet;
    FD_ZERO(&socketSet);
    FD_SET(listening, &socketSet);
    while(1)
    {
        fd_set sockSetCopy = socketSet;
        int count = select(0, &sockSetCopy, nullptr, nullptr, nullptr);
        for (int i=0; i<count; ++i)
        {
            int socket=FD_ISSET(i, &sockSetCopy);
            if (socket==listening)
            {
                int client = accept (listening, nullptr, nullptr);
                FD_SET (client, &socketSet);
                char * start = "Connected to server successfully\n";
                send (client, start, sizeof(start), 0);
            }
            else
            {
                char rcvMess[1024];
                int rcvBytes= recv (socket, rcvMess, 1024, 0);
                if (rcvBytes<=0)
                {
                    close(socket);
                    FD_CLR(socket, &socketSet);
                }
                //Some annotation:
                //If rcvMess[0]='r'; //user want to register a username;
                //If rcvMess[0]='a' or 'b' or 'c' or 'd'; //user is answering;
                //If rcvMess[0]='s'; //user skip a question;
                //If rcvMess[0]= 'q' //user quit.
                //Username will be send in rcvMess!
                if (rcvMess[0]=='r')
                {
                   //Call registering action
                }
                else if (rcvMess[0]=='a' || rcvMess[0]=='b' || rcvMess[0]=='c' ||rcvMess[0] =='d')
                {
                    //Call check anwer action
                    //Determine if stay, out or winner
                }
                else if (rcvMess[0]=='s')
                {
                    //Pass question to next player
                }
                else if (rcvMess[0]= 'q')
                {
                    close(socket);
                    FD_CLR(socket, &socketSet);
                }
                else //this case user trying to send the username for registration
                {
                    //Call to check whether the username is valid or not
                }

            }
        }
    return 0;  
    }
}  