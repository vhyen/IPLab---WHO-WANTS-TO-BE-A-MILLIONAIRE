//
//  GamePlay.cpp
//  quizgame
//
//  Created by Yến  on 30/03/2023.
//

#include "GamePlay.hpp"


GamePlay::GamePlay()
{
    std::cout << "GamePlay constructor\n";
    numPlayerRemained = 0;
    currentPlayer = 0;
    currentQuestion = 0;
    isEnd = false;
    winner = "";
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
    QuestionManager* questionManager = QuestionManager::getInstance();
    questions = questionManager->generateRandomQuestion(players.size());
}

// process management
int GamePlay::processPlayerChoice(int option) {
    if (option == 0) {
        // xu ly player da xai move roi?
        players[currentPlayer].haveMove = false;
        return 1;
    }
    else {
        if (!checkAnswer(option)) disqualifyCurrentPlayer();
        currentQuestion++;
    }
    if (currentQuestion >= questions.size()) {
        winner = players[currentPlayer].username;
        isEnd = true;
        return 1;
    } 
    processNextPlayer();
    return 1;
}

void GamePlay::processMoveTurn() {

}

void GamePlay::processNextPlayer() {
    currentPlayer++;
    while (!players[currentPlayer].isQualified) {
        currentPlayer++;
        if (currentPlayer == players.size()) currentPlayer = 0;
    }
}

bool GamePlay::isEndGame() {
    return isEnd;
}

std::string GamePlay::getWinner() {
    
}

// player management
void GamePlay::disqualifyCurrentPlayer() {
    players[currentPlayer].isQualified = false;
    numPlayerRemained--;
    if (numPlayerRemained == 1)  {
        isEnd = true;
        for (Player pl : players)
            if (pl.isQualified) winner = pl.username;
    }
}

int GamePlay::moveTurn() {
    if (players[currentPlayer].haveMove) {
        players[currentPlayer].haveMove = false;
        return 1;
    }
    return 0;
}

std::string GamePlay::nextPlayer() {
    return players[currentPlayer].username;
}

bool GamePlay::checkAnswer(int ans) {
    std::string convertedAnswer;
    switch (ans) {
        case 1: convertedAnswer = "A"; break;
        case 2: convertedAnswer = "B"; break;
        case 3: convertedAnswer = "C"; break;
        case 4: convertedAnswer = "D"; break;
    }
    if (convertedAnswer == questions[currentQuestion].answer) return true;
    return false;
}


Question GamePlay::getQuestion() {
    Question q = Question();
    q.question = questions[currentQuestion].question;
    q.options.assign(questions[currentQuestion].options.begin(), questions[currentQuestion].options.end());
    q.answer = "";
    return q;
}




/*
start
-> chon nguoi tra loi
-> chon cau hoi

loop:
-> gui toi clients
-> nhan action cua client
->
    + move turn: 
        giu nguyen cau hoi
        chon nguoi tra loi tiep theo
    + tra loi:
        xac nhan cau tra loi
        chon cau hoi tiep theo
        chon nguoi tra loi tiep theo
*/