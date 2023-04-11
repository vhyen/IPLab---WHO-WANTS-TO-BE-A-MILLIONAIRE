#include <sys/types.h>
#include <sys/socket.h>

#include <map>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <mutex>

#include "GamePlay.hpp"

#define MAX_LEN 200
#define NUM_COLORS 6
#define PORT 8989



struct terminal
{
	int id;
	std::string name;
	int socket;
	std::thread th;
};

std::vector<terminal> clients;
std::string def_col = "\033[0m";
std::string colors[] = {"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m"};
int seed = 0;
std::mutex cout_mtx, clients_mtx;

bool isInGame;
GamePlay gameplay;

std::string color(int code);
void set_name(int id, char name[]);
void shared_print(std::string str, bool endLine);
int broadcast_message(std::string message, int sender_id);
int broadcast_message(int num, int sender_id);
void end_connection(int id);
void handle_client(int client_socket, int id);
void runGame();

int main()
{
	struct sockaddr_in server;
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	int option = 1;
	if (setsockopt(server_socket, SOL_SOCKET,
				   SO_REUSEADDR | SO_REUSEPORT, &option,
				   sizeof(option)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);
	// bzero(&server.sin_zero,0);

	if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	std::cout << server.sin_port << std::endl;

	if ((listen(server_socket, 5)) == 0)
	{
		std::cout << "Listening\n";
	}
	else
	{
		perror("listen error: ");
		exit(-1);
	}

	struct sockaddr_in client;
	int client_socket;
	unsigned int len = sizeof(sockaddr_in);

	std::cout << colors[NUM_COLORS - 1] << "\n\t  ====== Welcome to the chat-room ======   " << std::endl
		 << def_col;

	while (1)
	{
		// accept new socket
		if ((client_socket = accept(server_socket, (struct sockaddr *)&client, &len)) == -1)
		{
			perror("accept error: ");
			exit(-1);
		}
		seed++;
		std::thread t(handle_client, client_socket, seed);
		std::lock_guard<std::mutex> guard(clients_mtx);
		clients.push_back({seed, std::string("Anonymous"), client_socket, (move(t))});
	}

	for (int i = 0; i < clients.size(); i++)
	{
		if (clients[i].th.joinable())
			clients[i].th.join();
	}

	close(server_socket);
	return 0;
}

std::string color(int code)
{
	return colors[code % NUM_COLORS];
}

// Set name of client
void set_name(int id, char name[])
{
	for (int i = 0; i < clients.size(); i++)
	{
		if (clients[i].id == id)
		{
			clients[i].name = string(name);
		}
	}
}

// For synchronisation of cout statements
void shared_print(std::string str, bool endLine = true)
{
	lock_guard<mutex> guard(cout_mtx);
	cout << str;
	if (endLine)
		cout << endl;
}

// Broadcast message to all clients except the sender
int broadcast_message(std::string message, int sender_id)
{
	char temp[MAX_LEN];
	strcpy(temp, message.c_str());
	for (int i = 0; i < clients.size(); i++)
	{
		if (clients[i].id != sender_id)
		{
			send(clients[i].socket, temp, sizeof(temp), 0);
		}
	}
}

// Broadcast a number to all clients except the sender
int broadcast_message(int num, int sender_id)
{
	for (int i = 0; i < clients.size(); i++)
	{
		if (clients[i].id != sender_id)
		{
			send(clients[i].socket, &num, sizeof(num), 0);
		}
	}
}

void end_connection(int id)
{
	for (int i = 0; i < clients.size(); i++)
	{
		if (clients[i].id == id)
		{
			std::lock_guard<std::mutex> guard(clients_mtx);
			clients[i].th.detach();
			clients.erase(clients.begin() + i);
			close(clients[i].socket);
			break;
		}
	}
}

void handle_client(int client_socket, int id)
{
	char name[MAX_LEN], str[MAX_LEN];

	// handle username
	bool isValidUsername = false;
	while (!isValidUsername)
	{
		char name[MAX_LEN];
		recv(client_socket, name, sizeof(name), 0);
		std::string inputName = std::string(name);
		std::cout << "checking " << inputName << std::endl;
		int i = 0;
		for (i; i < clients.size(); i++)
		{
			if (clients[i].name == inputName)
			{
				isValidUsername = false;
				std::cout << "already existed\n";
				std::string message = "NO";
				char sent[MAX_LEN];
				strcpy(sent, message.c_str());
				send(client_socket, sent, sizeof(sent), 0);
				break;
			}
		}
		if (i == clients.size())
			isValidUsername = true;
	}
	std::string message = "OK";
	char sent[MAX_LEN];
	strcpy(sent, message.c_str());
	send(client_socket, sent, sizeof(sent), 0);
	set_name(id, name);

	while (1)
	{
		int bytes_received = recv(client_socket, str, sizeof(str), 0);
		if (bytes_received <= 0)
			return;
		if (strcmp(str, "#exit") == 0)
		{
			// Display leaving message
			std::string message = std::string(name) + std::string(" has left");
			broadcast_message("#NULL", id);
			broadcast_message(id, id);
			broadcast_message(message, id);
			shared_print(color(id) + message + def_col);
			end_connection(id);
			return;
		}
		broadcast_message(std::string(name), id);
		broadcast_message(id, id);
		broadcast_message(std::string(str), id);
		shared_print(color(id) + name + " : " + def_col + str);
	}
}

void runGame()
{
	// init gameplay
	if (clients.size() == 5)
		isInGame = true;
	std::vector<std::string> usernames;
	std::map<std::string, int> user_index;
	for (int i = 0; i < clients.size(); i++)
	{
		usernames.push_back(clients[i].name);
		user_index[clients[i].name] = i;
	}
	// gameplay.initPlayers(usernames);
	// gameplay.initQuestions();
	gameplay.startGame(usernames);
}
