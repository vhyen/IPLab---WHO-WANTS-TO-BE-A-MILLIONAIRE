#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
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
#include "Question.hpp"
#include "Play.hpp"

#define MAX_LEN 800
#define NUM_COLORS 6
#define PORT 10000

using namespace std;


bool exit_flag=false;
thread t_send, t_recv;
int client_socket;
string def_col="\033[0m";
string colors[]={"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m"};
char separator = '|';

// render
sf::RenderWindow window(sf::VideoMode(2560, 1440), "Who wants to be millionaire?");
Register rgt(&window);
// game control
Play gameplay;
bool isInGame;

void catch_ctrl_c(int signal);
string color(int code);
int eraseText(int cnt);
void send_message(int client_socket);
void sendMessage(int client_socket, string message);
void recv_message(int client_socket);
void processGameplay(string message);
void getPlayerInfo(string message);
vector<string> splitMessage(string& str, char separator);

int main()
{
	string messageOK = "OK";
	isInGame = false;
	gameplay.setWindow(&window);

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
	cout << "Done register\n";
	gameplay.setUsername(username);

	// rgt.reset();
	// rgt.render();

	recv(client_socket, response, sizeof(response), 0);
	cout << string(response) << endl;
	while(string(response) != "startgame") {
		recv(client_socket, response, sizeof(response), 0);
	}
	recv(client_socket, response, sizeof(response), 0);
	isInGame = true;
	string str = string(response);
	vector<string> usernames = splitMessage(str, separator);
	gameplay.initPlayers(usernames);
	sendMessage(client_socket, string("OK"));
	recv(client_socket, response, sizeof(response), 0);
	cout << response << endl;
	
	// =================================== STOP ===============================

	cout<<colors[NUM_COLORS-1]<<"\n\t  ====== Welcome to the chat-room ======   "<<endl<<def_col;

	cout << "=================================================\n";
	char message[MAX_LEN];
	while(1)
	{
		char message_temp[MAX_LEN];
		if(exit_flag)
			return 0;
		int color_code = 0;
		int bytes_received=recv(client_socket, message_temp,sizeof(message_temp), 0);
		if (bytes_received > 0) {
			// strcpy(message, message_temp);
			processGameplay(string(message_temp));		
		}
		fflush(stdout);
	}
	
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

void sendMessage(int client_socket, string message)
{
		char str[MAX_LEN];
		strcpy(str, message.c_str());
		send(client_socket, str, sizeof(str), 0);
		fflush(stdout);
}

// Receive message
void recv_message(int client_socket)
{
	cout << "recv_message" << endl;
	char message[MAX_LEN];
	while(1)
	{
		char message_temp[MAX_LEN];
		if(exit_flag)
			return;
		int color_code = 0;
		int bytes_received=recv(client_socket, message_temp,sizeof(message_temp), 0);
		if (bytes_received > 0) {
			strcpy(message, message_temp);
		}
		processGameplay(message);		
		fflush(stdout);
	}
}

void processGameplay(string message) {
	cout << "Process gameplay: " << message << endl;
	vector<string> parts = splitMessage(message, separator);
	string currentPlayer = parts[0];
	Question q;
	q.question = parts[1];
	q.optionA = parts[2];
	q.optionB = parts[3];
	q.optionC = parts[4];
	q.optionD = parts[5];

	gameplay.getPlayerAndQuestion(currentPlayer, q);
	gameplay.render();
	string option = gameplay.getChosenOption(); // get option from game
	char ans[MAX_LEN];
	strcpy(ans, option.c_str());
	send(client_socket, ans, sizeof(ans), 0);
}


void getPlayerInfo(string message) {
	vector<string> players = splitMessage(message, '|');
	gameplay.initPlayers(players);
}

vector<string> splitMessage(string& str, char separator) {
    int startIndex = 0, endIndex = 0;
    vector<string> strings;
    for (int i = 0; i <= str.size(); i++) {
        if (str[i] == separator || i == str.size()) {
            endIndex = i;
            string temp;
            temp.append(str, startIndex, endIndex - startIndex);
            strings.push_back(temp);
            startIndex = endIndex + 1;
        }
    }
    return strings;
}


