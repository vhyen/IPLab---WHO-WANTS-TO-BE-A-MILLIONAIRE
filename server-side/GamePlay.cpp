#include <GamePlay.h>
#include <fstream>
#include <iostream>

GamePlay::GamePlay() {
    std::cout << "GamePlay constructor\n";
    players = {};
    noPlayerRemained = 0;
    currentPlayer = 0;

    questions = {};
    noQuestionRemained = 0;
}


void GamePlay::initPlayers(std::vector<std::string> usernames) {
    std::cout << "GamePlay init players\n";
    for (std::string username : usernames) {
        players.push_back(Player(username));
    }
    noPlayerRemained = players.size();
    currentPlayer = 0;
}

void GamePlay::initQuestions() {
    std::cout << "GamePlay init questions\n";
    std::string filename = "questions.csv";
    std::string line;

    // get random questions from files
    
    // 

    noQuestionRemained = questions.size();
}