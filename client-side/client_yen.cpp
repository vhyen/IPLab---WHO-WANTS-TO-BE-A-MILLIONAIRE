#include <iostream>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <signal.h>
#include <mutex>

#include "SFML/Graphics.hpp"
#include "Register.hpp"


#define MAX_LEN 200
#define NUM_COLORS 6
#define PORT 50894

using namespace std;

// message header
typedef char message_header;
const message_header ASK_NICKNAME = 0;
const message_header NICKNAME_OK = 1;
const message_header ANSWER_NICKNAME = 2;
const message_header ANSWER_QUESTION = 3;
const message_header QUESTION = 4;
const message_header GAME_START = 5;
const message_header GAME_FINISH = 6;
const message_header RESULTS_OF_SET = 7;
const message_header YOU_WIN = 8;

bool exit_flag=false;
thread t_send, t_recv;
int client_socket;
string def_col="\033[0m";
string colors[]={"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m"};

// render
sf::RenderWindow window(sf::VideoMode(2560, 1440), "Who wants to be millionaire?");
Register rgt(&window);
// game control
bool isInGame;

void catch_ctrl_c(int signal);
string color(int code);
int eraseText(int cnt);
void send_message(int client_socket);
void recv_message(int client_socket);

int main()
{

	if((client_socket=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket: ");
		exit(-1);
	}

	struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=INADDR_ANY;
	client.sin_port=htons(PORT);
	bzero(&client.sin_zero,0);

	if((connect(client_socket,(struct sockaddr *)&client,sizeof(struct sockaddr_in)))==-1)
	{
		perror("connect: ");
		exit(-1);
	}
	
	signal(SIGINT, catch_ctrl_c);
	char name[MAX_LEN];

	rgt.render();
	string username = rgt.getUsername();
	strcpy(name, username.c_str());

	send(client_socket,name,sizeof(name),0);

	char response[MAX_LEN];
	recv(client_socket,response,sizeof(response),0);
	cout << "response: " << string(response) << endl;
	bool existedUsername = (string(response) == "OK") ? false : true;
	while (existedUsername) {
		rgt.existedUsername();
		rgt.render();
		string username = rgt.getUsername();
		strcpy(name, username.c_str());
		send(client_socket,name,sizeof(name),0);
		char response[MAX_LEN];
		recv(client_socket,response,sizeof(response),0);
		cout << "response: " << string(response) << endl;
		existedUsername = (string(response) == "OK") ? false : true;
	}
	
	rgt.reset();
	rgt.render();

	char str[MAX_LEN];
	string exit = "#exit";
	strcpy(str, exit.c_str());
	send(client_socket,str,sizeof(str),0);

	return 0;

	// =================================== STOP ===============================

	cout<<colors[NUM_COLORS-1]<<"\n\t  ====== Welcome to the chat-room ======   "<<endl<<def_col;

	thread t1(send_message, client_socket);
	thread t2(recv_message, client_socket);

	t_send=move(t1);
	t_recv=move(t2);

	if(t_send.joinable())
		t_send.join();
	if(t_recv.joinable())
		t_recv.join();
			
	return 0;
}

// Handler for "Ctrl + C"
void catch_ctrl_c(int signal) 
{
	char str[MAX_LEN]="#exit";
	send(client_socket,str,sizeof(str),0);
	exit_flag=true;
	t_send.detach();
	t_recv.detach();
	close(client_socket);
	exit(signal);
}

string color(int code)
{
	return colors[code%NUM_COLORS];
}

// Erase text from terminal
int eraseText(int cnt)
{
	char back_space=8;
	for(int i=0; i<cnt; i++)
	{
		cout<<back_space;
	}	
}

// Send message to everyone
void send_message(int client_socket)
{
	while(1)
	{
		cout<<colors[1]<<"You : "<<def_col;
		char str[MAX_LEN];
        // str = question.to_message();
		cin.getline(str,MAX_LEN);
		send(client_socket,str,sizeof(str),0);
		if(strcmp(str,"#exit")==0 || !window.isOpen())
		{
			exit_flag=true;
			t_recv.detach();	
			close(client_socket);
			return;
		}	
	}		
}

// Receive message
void recv_message(int client_socket)
{
	while(1)
	{
		if(exit_flag)
			return;
		char header[MAX_LEN], body[MAX_LEN];
		int color_code = 0;
		int bytes_received=recv(client_socket, header,sizeof(header), 0);
		if(bytes_received<=0)
			continue;

		recv(client_socket,body,sizeof(body),0); 


		eraseText(6);
		if(strcmp(header,"#NULL")!=0)
			cout << header <<endl;
		else
			cout<<color(color_code)<<body<<endl;
		cout<<colors[1]<<"You : "<<def_col; 
		fflush(stdout);
	}
}