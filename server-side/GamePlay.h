#include <iostream>
#include <string>
#include <vector>
#include <Player.h>
#include <Question.h>
class GamePlay
{
private:
    std::vector<Player> players;
    int noPlayerRemained; // number of qualified players
    int currentPlayer; // keep track of answer turn for the player, currently in ascending order, no randomized

    std::vector<Question> questions;
    int noQuestionRemained; // number of question remain

public:
    // constructors
    GamePlay();
    // destructor ??

    // initialization
    void initPlayers(std::vector<std::string> usernames);
    void initQuestions(); 
    
    // process management
    void processPlayerAnswer();
    void processMoveTurn();
    void processNextTurn(); 
    void endGamePlay();

    // player management
    void disqualifyPlayer();
    void moveTurn();

    // question management
    int getNumOfQuestions();
    std::vector<std::string> getPlayers(); // trả về usernames
    void getNextQuestion(); // cái hàm này trả về câu hỏi và 4 câu trả lời
    bool checkAnswer(int questIndex); 

}

// Game play
// when created, stored list of players and list of questions