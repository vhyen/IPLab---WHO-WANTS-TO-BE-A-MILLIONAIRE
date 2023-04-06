//
//  GamePlay.hpp
//  quizgame
//
//  Created by Yến  on 30/03/2023.
//

#ifndef GamePlay_hpp
#define GamePlay_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "Player.hpp"
#include "QuestionManager.hpp"

class GamePlay
{
private:
    std::vector<Player> players;
    int numPlayerRemained; 
    // number of qualified players
    int currentPlayer; 
    // keep track of answer turn for the player, 
    // currently in ascending order, no randomized

    std::vector<Question> questions;
    int currentQuestion; // number of question remain
    bool isEnd;
    std::string winner;

public:
    // constructors
    GamePlay();
    // destructor ??

    // initialization
    void initPlayers(std::vector<std::string> usernames);
    void initQuestions();
    //bool registerOK(std::string name);
    
    // process management
    void startGame(std::vector<std::string> usernames);
    int processPlayerChoice(int option);
    void processMoveTurn();
    void processNextPlayer();
    bool isEndGame();
    std::string getWinner();

    // player management
    void disqualifyCurrentPlayer();
    int moveTurn();
    std::string nextPlayer();

    // question management
    int getNumOfQuestions();
    std::vector<std::string> getPlayers(); // trả về usernames
    Question getQuestion(); // cái hàm này trả về câu hỏi và 4 câu trả lời
    bool checkAnswer(int ans);

};


#endif /* GamePlay_hpp */
