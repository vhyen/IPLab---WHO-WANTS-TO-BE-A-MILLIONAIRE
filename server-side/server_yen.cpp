#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <mutex>

#include "GamePlay.hpp"
#include "Question.hpp"
#include "QuestionManager.hpp"

#define MAX_LEN 200
#define NUM_COLORS 6
#define PORT 10000

// =========== SOCKET =========== //
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

// =========== MESSAGE =========== //

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

// =========== GAMEPLAY =========== //

QuestionManager *QuestionManager::instance = NULL;
GamePlay gameplay;

// =========== FUNCTIONS =========== //

std::string color(int code);
void set_name(int id, char name[]);
void shared_print(std::string str, bool endLine);
int broadcast_message(std::string message, int sender_id);
int broadcast_message(int num, int sender_id);
void end_connection(int id);
void handle_client(int client_socket, int id);
void sendQuestion();

// =========== MAIN =========== //

int main()
{
	QuestionManager *questionMgr = QuestionManager::getInstance();
	questionMgr->retrieveQuestionsFromFile("QuestData.txt");

	struct sockaddr_in server;
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	int option = 1;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

	bind(server_socket, (struct sockaddr *)&server, sizeof(server));

	std::cout << server.sin_port << std::endl;

	if ((listen(server_socket, 10)) == 0)
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

	std::cout << colors[NUM_COLORS - 1] << "\n\t  ====== Welcome to the game server ======   " << std::endl
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
			clients[i].name = std::string(name);
		}
	}
}

// For 
void shared_print(std::string str, bool endLine = true)
{
	std::lock_guard<std::mutex> guard(cout_mtx);
	std::cout << str;
	if (endLine)
		std::cout << std::endl;
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
	return 1;
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
	return 1;
}

// Broadcast message to all clients
int broadcast_message(std::string message)
{
	std::cout << "BROADCAST: " << message << std::endl;
	char temp[MAX_LEN];
	strcpy(temp, message.c_str());
	for (int i = 0; i < clients.size(); i++)
	{
		send(clients[i].socket, temp, sizeof(temp), 0);
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
		// char name[MAX_LEN];
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

	std::cout << "CLIENTS: ";
	for (int i = 0; i<clients.size(); i++) {
		std::cout << clients[i].name << " "; 
	}
	std::cout << std::endl;

	// if clients == 3, start game from here
	if (gameplay.isEndGame() && clients.size() == 2)
	{
		// get players
		std::vector<std::string> usernames;
		for (int i = 0; i < clients.size(); i++)
		{
			usernames.push_back(clients[i].name);
		}
		// init gameplay
		gameplay.startGame(usernames);
		std::string startMessage = "startgame";
		broadcast_message(startMessage);

		std::string playerInfo = "";
		// playerInfo = to_string(clients.size());
		for (int i = 0; i < clients.size(); i++)
		{
			playerInfo += clients[i].name;
			if (i != clients.size() - 1)
				playerInfo += '|';
		}
		broadcast_message(playerInfo);
		char response[MAX_LEN];
		while (std::string(response) != "OK") recv(client_socket, response, sizeof(response), 0);
		std::cout << response << std::endl;
		broadcast_message("OK");
		// process first turn
		sendQuestion();
	}

	// process receiving event from client
	while (1)
	{
		int bytes_received = recv(client_socket, str, sizeof(str), 0);
		if (bytes_received <= 0)
			return;

		std::cout << std::string(str) << std::endl;

		// exit
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

		std::string choice = std::string(str);
		// gameplay process
		if (!gameplay.isEndGame())
		{
			if (choice ==  "A" || choice == "B" || choice == "C" || choice == "D" || choice == "move") {
				gameplay.processPlayerChoice(choice);
			if (!gameplay.isEndGame())
			{
				sendQuestion();
			}
			else
			{
				broadcast_message("endgame");
				//broadcast_message(gameplay.getWinner());
			}
			}
				
		}
	}
}

// get next turn from gameplay and send message to all users
// message format: answerer|question|optionA|optionB|optionC|optionD
void sendQuestion()
{
	//std::string message = gameplay.getPlayerAndQuestion();
	std::string message = "star|q|a|b|c|d";
	broadcast_message(message);
}