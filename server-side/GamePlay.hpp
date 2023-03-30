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
#include "Player.hpp"
#include "Question.hpp"

class GamePlay
{
private:
    std::vector<Player> players;
    int numPlayerRemained; // number of qualified players
    int currentPlayer; // keep track of answer turn for the player, currently in ascending order, no randomized

    std::vector<Question> questions;
    int numQuestionRemained; // number of question remain

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

};


#endif /* GamePlay_hpp */
