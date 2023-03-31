//
//  GamePlay.cpp
//  quizgame
//
//  Created by Yến  on 30/03/2023.
//

#include "GamePlay.hpp"
#include <fstream>
#include <vector>
#include <iostream>
#include <fstream>

GamePlay::GamePlay()
{
    std::cout << "GamePlay constructor\n";
    // players = {};
    numPlayerRemained = 0;
    currentPlayer = 0;

    // questions = {};
    numQuestionRemained = 0;
}

void GamePlay::initPlayers(std::vector<std::string> usernames)
{
    std::cout << "GamePlay init players\n";
    for (std::string username : usernames)
    {
        Player player = Player(username);
        players.push_back(player);
    }
    numPlayerRemained = int(players.size());
    currentPlayer = 0;
    std::cout << "Number of players in this gameplay: " << players.size() << std::endl;
}

void GamePlay::initQuestions()
{
    
    // quest tra ve random cau hoi

    numQuestionRemained = int(questions.size());
}