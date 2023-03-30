// #include <iostream>
// #include <vector>
// #include <cstdlib>
// #include <ctime>
// #include <chrono>
// #include <thread>
// #include <winsock2.h>

// // Constants
// const int MAX_CLIENTS = 10;
// const int PORT = 12345;
// const int TIMEOUT_SECONDS = 30;

// // Question struct
// struct Question
// {
//     std::string question;
//     std::string answer;
// };

// // Function to load questions from a file
// std::vector<Question> load_questions()
// {
//     std::vector<Question> questions;
//     // Load questions from a file and populate the questions vector

//     return questions;
// }

// // Function to send a message to all clients
// void send_to_all_clients(SOCKET clients[], int num_clients, const char *message)
// {
//     for (int i = 0; i < num_clients; i++)
//     {
//         send(clients[i], message, strlen(message), 0);
//     }
// }

// // Function to handle a client
// void handle_client(SOCKET client_socket, int player_num, std::vector<Question> &questions)
// {
//     // Send welcome message to client
//     std::string welcome_message = "Welcome to Who Wants to Be a Millionaire! You are player " + std::to_string(player_num) + ".\n";
//     send(client_socket, welcome_message.c_str(), welcome_message.size(), 0);

//     // Wait for all clients to connect
//     if (player_num == 1)
//     {
//         std::string waiting_message = "Waiting for all players to connect...\n";
//         send_to_all_clients(&client_socket, 1, waiting_message.c_str());
//     }

//     // Wait for all clients to connect before starting the game
//     while (true)
//     {
//         fd_set set;
//         FD_ZERO(&set);
//         FD_SET(client_socket, &set);

//         struct timeval timeout;
//         timeout.tv_sec = TIMEOUT_SECONDS;
//         timeout.tv_usec = 0;

//         int select_result = select(1, &set, nullptr, nullptr, &timeout);
//         if (select_result == SOCKET_ERROR)
//         {
//             std::cerr << "Error during select(): " << WSAGetLastError() << std::endl;
//             break;
//         }
//         else if (select_result == 0)
//         {
//             // Timeout occurred, not all clients have connected
//             std::string timeout_message = "Timeout occurred. Not all players have connected.\n";
//             send_to_all_clients(&client_socket, 1, timeout_message.c_str());
//             break;
//         }
//         else
//         {
//             // Client has connected
//             char buffer[1024];
//             recv(client_socket, buffer, sizeof(buffer), 0);
//             break;
//         }
//     }

//     // Start the game
//     int num_players = MAX_CLIENTS;
//     std::string start_message = "Game starting with " + std::to_string(num_players) + " players!\n";
//     send_to_all_clients(&client_socket, 1, start_message.c_str());

//     // Shuffle the questions
//     std::srand(std::time(nullptr));
//     std::random_shuffle(questions.begin(), questions.end());

//     // Loop through the questions and ask them to the players
//     for (int i = 0; i < questions.size(); i++)
//     {
//         // Send the question to all clients
//         std::string question_message = "Question " + std::to_string(i + 1) + ": " + questions[i].question + "\n";
//         send_to_all_clients(&client_socket, 1, question_message.c_str());

//         // Wait for the players to answer
//         bool all_answered = false;
//         while (!all_answered)
//         {
//             // Wait for a response from one of the clients
//             fd_set set;
//             FD_ZERO(&set);
//             FD_SET(client_socket, &set);

//             Copy struct timeval timeout;
//             timeout.tv_sec = TIMEOUT_SECONDS;
//             timeout.tv_usec = 0;

//             int select_result = select(1, &set, nullptr, nullptr, &timeout);
//             if (select_result == SOCKET_ERROR)
//             {
//                 std::cerr << "Error during select(): " << WSAGetLastError() << std::endl;
//                 break;
//             }
//             else if (select_result == 0)
//             {
//                 // Timeout occurred, not all players have answered
//                 std::string timeout_message = "Timeout occurred. Not all players have answered the question.\n";
//                 send_to_all_clients(&client_socket, 1, timeout_message.c_str());
//                 break;
//             }
//             else
//             {
//                 char buffer[1024];
//                 recv(client_socket, buffer, sizeof(buffer), 0);
//                 std::string answer = buffer;

//                 // Check if the answer is correct
//                 bool is_correct = (answer == questions[i].answer);

//                 // Send the result to all clients
//                 std::string result_message = "Player " + std::to_string(player_num) + " answered: " + answer + ". ";
//                 if (is_correct)
//                 {
//                     result_message += "Correct!\n";
//                 }
//                 else
//                 {
//                     result_message += "Incorrect.\n";
//                 }
//                 send_to_all_clients(&client_socket, 1, result_message.c_str());

//                 // Wait for a short delay before sending the next question
//                 std::this_thread::sleep_for(std::chrono::seconds(2));

//                 // Check if all players have answered the question
//                 all_answered = true;
//                 for (int j = 0; j < num_players; j++)
//                 {
//                     if (j != player_num - 1)
//                     {
//                         fd_set set;
//                         FD_ZERO(&set);
//                         FD_SET(clients[j], &set);

//                         struct timeval timeout;
//                         timeout.tv_sec = 0;
//                         timeout.tv_usec = 0;

//                         int select_result = select(1, &set, nullptr, nullptr, &timeout);
//                         if (select_result > 0)
//                         {
//                             // Another player has answered
//                             char buffer[1024];
//                             recv(clients[j], buffer, sizeof(buffer), 0);
//                             std::string answer = buffer;
//                             all_answered = false;
//                             break;
//                         }
//                     }
//                 }
//             }
//         }

//         // Check if there is only one player left
//         int num_active_players = 0;
//         for (int j = 0; j < num_players; j++)
//         {
//             fd_set set;
//             FD_ZERO(&set);
//             FD_SET(clients[j], &set);

//             struct timeval timeout;
//             timeout.tv_sec = 0;
//             timeout.tv_usec = 0;

//             int select_result = select(1, &set, nullptr, nullptr, &timeout);
//             if (select_result > 0)
//             {
//                 num_active_players++;
//             }
//         }
//         if (num_active_players == 1)
//         {
//             // Only one player left, end the game
//             break;
//         }
//     }

//     // Send the final message to all clients
//     std::string end_message = "Game over!\n";
//     send_to_all_clients(&client_socket, 1, end_message.c_str());
// }

// int main()
// {
//     // Initialize Winsock
//     WSADATA wsa_data;
//     int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
//     if (result != 0)
//     {
//         std::cerr << "WSAStartup failed: " << result << std::endl;
//         return 1;
//     }

//     // Create the server socket
//     SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//     if (server_socket == INVALID_SOCKET)
//     {
//         std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
//         WSACleanup();
//         return 1;
//     }

//     Copy
//         // Bind the server socket to a port
//         sockaddr_in server_address;
//     server_address.sin_family = AF_INET;
//     server_address.sin_addr.s_addr = INADDR_ANY;
//     server_address.sin_port = htons(PORT);

//     result = bind(server_socket, (sockaddr *)&server_address, sizeof(server_address));
//     if (result == SOCKET_ERROR)
//     {
//         std::cerr << "Error binding socket: " << WSAGetLastError() << std::endl;
//         closesocket(server_socket);
//         WSACleanup();
//         return 1;
//     }

//     // Listen for incoming connections
//     result = listen(server_socket, MAX_CLIENTS);
//     if (result == SOCKET_ERROR)
//     {
//         std::cerr << "Error listening for connections: " << WSAGetLastError() << std::endl;
//         closesocket(server_socket);
//         WSACleanup();
//         return 1;
//     }

//     // Load the questions from a file
//     std::vector<Question> questions = load_questions();

//     // Accept incoming connections from clients
//     SOCKET clients[MAX_CLIENTS];
//     int num_clients = 0;

//     while (num_clients < MAX_CLIENTS)
//     {
//         std::cout << "Waiting for client connection..." << std::endl;

//         // Accept the connection
//         sockaddr_in client_address;
//         int address_size = sizeof(client_address);
//         clients[num_clients] = accept(server_socket, (sockaddr *)&client_address, &address_size);
//         if (clients[num_clients] == INVALID_SOCKET)
//         {
//             std::cerr << "Error accepting connection: " << WSAGetLastError() << std::endl;
//             continue;
//         }

//         // Handle the client in a separate thread
//         std::thread t(handle_client, clients[num_clients], num_clients + 1, std::ref(questions));
//         t.detach();

//         // Increment the number of clients
//         num_clients++;
//     }

//     // Close the server socket and cleanup Winsock
//     closesocket(server_socket);
//     WSACleanup();

//     return 0;
// }
