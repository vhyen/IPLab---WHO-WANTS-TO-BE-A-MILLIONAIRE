#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <mutex>

#include "GamePlay.hpp"
#include "QuestionManager.hpp"

#define MAX_LEN 200
#define NUM_COLORS 6
#define PORT 10000

using namespace std;



// =========== SOCKET =========== //
struct terminal
{
	int id;
	string name;
	int socket;
	thread th;
};

vector<terminal> clients;
string def_col="\033[0m";
string colors[]={"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m","\033[36m"};
int seed=0;
mutex cout_mtx,clients_mtx;

// =========== GAMEPLAY =========== //

QuestionManager* QuestionManager::instance = NULL;
GamePlay gameplay;

// =========== FUNCTIONS =========== //

string color(int code);
void set_name(int id, char name[]);
void shared_print(string str, bool endLine);
int broadcast_message(string message, int sender_id);
int broadcast_message(int num, int sender_id);
void end_connection(int id);
void handle_client(int client_socket, int id);

// =========== MAIN =========== //

int main()
{
	QuestionManager* questionMgr = QuestionManager::getInstance();
	questionMgr->retrieveQuestionsFromFile("QuestData.txt");
	
	struct sockaddr_in server;
	int server_socket;
    server_socket=socket(AF_INET,SOCK_STREAM,0);
	server.sin_family=AF_INET;
	server.sin_port=htons(8080);
	server.sin_addr.s_addr=htonl(INADDR_ANY);
	//bzero(&server.sin_zero,0);

	int option = 1;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

	bind(server_socket,(struct sockaddr*)&server,sizeof(server));

	cout << server.sin_port << endl;

	if((listen(server_socket,10))==0)
	{
        cout << "Listening\n";
	}
    else {
		perror("listen error: ");
		exit(-1);
    }

	struct sockaddr_in client;
	int client_socket;
	unsigned int len=sizeof(sockaddr_in);

	cout<<colors[NUM_COLORS-1]<<"\n\t  ====== Welcome to the game server ======   "<<endl<<def_col;

	while(1)
	{
		// accept new socket
		if((client_socket=accept(server_socket,(struct sockaddr *)&client,&len))==-1)
		{
			perror("accept error: ");
			exit(-1);
		}
		seed++;
		thread t(handle_client,client_socket,seed);
		lock_guard<mutex> guard(clients_mtx);
		clients.push_back({seed, string("Anonymous"),client_socket,(move(t))});
	}

	for(int i=0; i<clients.size(); i++)
	{
		if(clients[i].th.joinable())
			clients[i].th.join();
	}

	close(server_socket);
	return 0;
}

string color(int code)
{
	return colors[code%NUM_COLORS];
}

// Set name of client
void set_name(int id, char name[])
{
	for(int i=0; i<clients.size(); i++)
	{
			if(clients[i].id==id)	
			{
				clients[i].name=string(name);
			}
	}	
}

// For synchronisation of cout statements
void shared_print(string str, bool endLine=true)
{	
	lock_guard<mutex> guard(cout_mtx);
	cout<<str;
	if(endLine)
			cout<<endl;
}

// Broadcast message to all clients except the sender
int broadcast_message(string message, int sender_id)
{
	char temp[MAX_LEN];
	strcpy(temp,message.c_str());
	for(int i=0; i<clients.size(); i++)
	{
		if(clients[i].id!=sender_id)
		{
			send(clients[i].socket,temp,sizeof(temp),0);
		}
	}
	return 1;		
}

// Broadcast a number to all clients except the sender
int broadcast_message(int num, int sender_id)
{
	for(int i=0; i<clients.size(); i++)
	{
		if(clients[i].id!=sender_id)
		{
			send(clients[i].socket,&num,sizeof(num),0);
		}
	}		
	return 1;
}

// Broadcast message to all clients
int broadcast_message(string message)
{
	char temp[MAX_LEN];
	strcpy(temp,message.c_str());
	for(int i=0; i<clients.size(); i++)
	{
		send(clients[i].socket,temp,sizeof(temp),0);
	}		
}


void end_connection(int id)
{
	for(int i=0; i<clients.size(); i++)
	{
		if(clients[i].id==id)	
		{
			lock_guard<mutex> guard(clients_mtx);
			clients[i].th.detach();
			clients.erase(clients.begin()+i);
			close(clients[i].socket);
			break;
		}
	}				
}

void handle_client(int client_socket, int id)
{
	char name[MAX_LEN],str[MAX_LEN];

	// handle username
	bool isValidUsername = false;
	while (!isValidUsername) {
		char name[MAX_LEN];
		recv(client_socket,name,sizeof(name),0);
		string inputName = string(name);
		cout << "checking " << inputName << endl;
		int i = 0;
		for (i; i < clients.size() ;i++) {
			if (clients[i].name == inputName) {
				isValidUsername = false;
				cout << "already existed\n";
				string message = "NO";
				char sent[MAX_LEN];
				strcpy(sent, message.c_str());
				send(client_socket, sent, sizeof(sent), 0);
				break;
			}
		}
		if (i == clients.size())isValidUsername = true;
	}
	string message = "OK";
	char sent[MAX_LEN];
	strcpy(sent, message.c_str());
	send(client_socket, sent, sizeof(sent), 0);
	set_name(id,name);	

	// if clients == 3, start game from here
	if (gameplay.isEndGame() && clients.size() == 3) {
		// get players
		vector<string> usernames;
		for (int i = 0; i < clients.size(); i++) {
			usernames.push_back(clients[i].name);
		}
		// init gameplay
		gameplay.startGame(usernames);
		string startMessage = "startgame";
		strcpy(sent, startMessage.c_str());
		broadcast_message(sent);

		// process first turn
		string answerer = gameplay.nextPlayer();
		Question question = gameplay.getQuestion();

	}

	// process receiving event from client
	while(1)
	{
		int bytes_received=recv(client_socket,str,sizeof(str),0);
		if(bytes_received<=0)
			return;
		if(strcmp(str,"#exit")==0)
		{
			// Display leaving message
			string message=string(name)+string(" has left");		
			broadcast_message("#NULL",id);			
			broadcast_message(id,id);						
			broadcast_message(message,id);
			shared_print(color(id)+message+def_col);
			end_connection(id);							
			return;
		}
		
		
		// broadcast_message(string(name),id);					
		// broadcast_message(id,id);		
		// broadcast_message(string(str),id);
		// shared_print(color(id)+name+" : "+def_col+str);		
	}	
}

