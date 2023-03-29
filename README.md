# IPLab---WHO-WANTS-TO-BE-A-MILLIONAIRE


### General View
1. Server Setup
* The server sets up the game by defining the number of players (N) that will participate in the game.
* The server also sets up a list of questions and their corresponding answers for the game.
2. Client Setup
* Each client connects to the server and waits for the game to start.
* Once all clients have connected, the server sends a message to all clients indicating that the game is starting.
3. Game Play
* The server selects a random question from the list and sends it to all clients.
* Each client has a set amount of time (e.g. 30 seconds) to answer the question.
* If a client answers correctly, server sends the next question and the corresponding client to answer that question.
* If a client answers incorrectly, they are disqualified for the game.
* The game continues until there is only one player left or all questions have been answered.
4. Game End
* Once the game is over, the server sends a message to all clients indicating who the winner is.
* The server then closes the connection to all clients, and start new session.
* Client can choose play again to rejoin the server or exit game.