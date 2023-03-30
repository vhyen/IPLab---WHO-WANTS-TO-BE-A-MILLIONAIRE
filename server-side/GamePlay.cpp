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
    std::cout << "GamePlay init questions\n";
    // random number of questions
    int num = int(players.size());
    srand(static_cast<unsigned int>(time(0)));
    numQuestionRemained = rand() % num + num * 3;
    std::cout << "number of questions: " << numQuestionRemained << std::endl;

    std::string filename = "questions.dat";
    std::vector<std::string> questData;
    std::string line;

    std::fstream file2("ac.txt", std::ios::out);
    file2 << "askljds";
    file2.close();

    // get random questions from files
    std::fstream file(filename, std::ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            questData.push_back(line);
        }
    }
    else
        std::cout << "Could not open the file\n";
    file.close();

    for (std::string each : questData)
    {
        std::cout << each << std::endl;
    }
    //

    numQuestionRemained = int(questions.size());
}