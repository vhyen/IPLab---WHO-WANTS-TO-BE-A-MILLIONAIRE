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
    isEnd = true;
    winner = "";
}
// bool GamePlay::registerOK(std::string name)
// {
//     bool check;
//     if (name.length()>10)
//     {
//         // To users: std::cout<<"Username is too long. Choose another\n";
//         return false;
//     }
//     for (int j=0; j<name.length(); ++j)
//     {
//         check=((name[j] >= 'a') && (name[j] <= 'z')) ||
//         ((name[j] >= 'A') && (name[j] <= 'Z')) || ((name[j] >= '0') && (name[j] <= '9')) || (name[j]=='_');
//         if (check==false)
//         {
//             // To users: std::cout<<"Disqualified username. Choose another (consists of upper/lower case letters and/or digits and/or underscore\n";
//             return false;
//         }
//     }
//     for (int i=0; i<players.size(); ++i)
//     {
//         if (name.compare(players[i].username)==0)
//         {
//             // To users: std::cout<<"Already registered username. Choose another\n";
//             return false;
//         }
//     }
//     // To users: std::cout<<"Successfully assigned username.\n";
//     return true;
// }

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
    QuestionManager *questionManager = QuestionManager::getInstance();
    questions = questionManager->generateRandomQuestion(2);
    // std::cout << "Number of questions: " << questions.size() << std::endl;
}

// process management

void GamePlay::startGame(std::vector<std::string> usernames)
{
    isEnd = false;
    initPlayers(usernames);
    initQuestions();
}

int GamePlay::processPlayerChoice(std::string option)
{
    if (currentQuestion == questions.size() - 1)
    {
        winner = players[currentPlayer].username;
        isEnd = true;
        return 1;
    }
    if (option == "move")
    {
        // xu ly player da xai move roi?
        players[currentPlayer].haveMove = false;
    }
    else
    {
        if (!checkAnswer(option))
            disqualifyCurrentPlayer();
        currentQuestion++;
    }
    processNextPlayer();
    return 1;
}

void GamePlay::processMoveTurn()
{
}

void GamePlay::processNextPlayer()
{
    currentPlayer = (currentPlayer == players.size() - 1) ? 0 : currentPlayer + 1;
    while (!players[currentPlayer].isQualified)
    {
        currentPlayer++;
        if (currentPlayer == players.size())
            currentPlayer = 0;
    }
}

bool GamePlay::isEndGame()
{
    return isEnd;
}

std::string GamePlay::getWinner()
{
    return winner;
}

void GamePlay::encodeSentQuestion(char message[]) {
    Question q = getQuestion();
    std::string str = nextPlayer() + "|";
    str += q.question;
    for (std::string each : q.options) {
        str += "|";
        str += each;
    }
    std::cout << str << std::endl;
    strcpy(message, str.c_str());
    // return str;
} 

std::string GamePlay::getPlayerAndQuestion() {
    Question q = getQuestion();
    std::string str = nextPlayer() + "|";
    str += q.question;
    for (std::string each : q.options) {
        str += "|";
        str += each;
    }
    std::cout << str << std::endl;
    return str;
}

// player management
void GamePlay::disqualifyCurrentPlayer()
{
    players[currentPlayer].isQualified = false;
    numPlayerRemained--;
    if (numPlayerRemained == 1)
    {
        isEnd = true;
        for (Player pl : players)
            if (pl.isQualified)
                winner = pl.username;
    }
}

int GamePlay::moveTurn()
{
    if (players[currentPlayer].haveMove)
    {
        players[currentPlayer].haveMove = false;
        return 1;
    }
    return 0;
}

std::string GamePlay::nextPlayer()
{
    return players[currentPlayer].username;
}

bool GamePlay::checkAnswer(std::string ans)
{
    std::string convertedAnswer = ans;
    if (convertedAnswer == questions[currentQuestion].answer)
        return true;
    return false;
}

// question management

Question GamePlay::getQuestion()
{
    Question q = Question();
    q.question = questions[currentQuestion].question;
    q.options.assign(questions[currentQuestion].options.begin(), questions[currentQuestion].options.end());
    q.answer = "";
    return q;
}



int GamePlay::getNumOfQuestions()
{
    return questions.size();
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