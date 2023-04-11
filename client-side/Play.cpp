#include "Play.hpp"
Play::Play()
{
    // initial status
    this->skip = false;
    this->join = false;
    this->stillQualified = false;
    this->optChosen = false;

    // background
    scrBackground.loadFromFile("../client-side/assets/images/backgrounds/quiz-background.png");
    background.setTexture(scrBackground);
    background.scale(sf::Vector2f(1.34, 1.34));
    background.setPosition(0, 0);

    // set up displaying question
    posQuestion = sf::Vector2f(300, 420);
    posOptA = sf::Vector2f(144, 762);
    posOptB = sf::Vector2f(1048, 762);
    posOptC = sf::Vector2f(144, 1040);
    posOptD = sf::Vector2f(1048, 1040);
    sizeOpt = sf::Vector2f(823, 154);

    int paddingx = 44, paddingy = 40;

    font.loadFromFile("../client-side/assets/fonts/Poppins-Medium.ttf");
    question.setFont(font);
    optA.setFont(font);
    optB.setFont(font);
    optC.setFont(font);
    optD.setFont(font);
    question.setFillColor(sf::Color::White);
    optA.setFillColor(sf::Color::White);
    optB.setFillColor(sf::Color::White);
    optC.setFillColor(sf::Color::White);
    optD.setFillColor(sf::Color::White);
    question.setCharacterSize(64);
    question.setPosition(posQuestion);
    optA.setCharacterSize(32);
    optB.setCharacterSize(32);
    optC.setCharacterSize(32);
    optD.setCharacterSize(32);
    optA.setPosition(posOptA.x + paddingx, posOptA.y + paddingy);
    optB.setPosition(posOptB.x + paddingx, posOptB.y + paddingy);
    optC.setPosition(posOptC.x + paddingx, posOptC.y + paddingy);
    optD.setPosition(posOptD.x + paddingx, posOptD.y + paddingy);
}

void Play::setWindow(sf::RenderWindow *wd)
{
    window = wd;
}

void Play::setUsername(std::string username)
{
    this->username = username;
}

void Play::Init()
{
}

void Play::Registered()
{
    std::cout << "Choose a nickname to play. Only letters, digits and underscore allowed!\n";
    std::string name;
    std::cin >> name;
    bool check;
    if (name.length() > 10)
    {
        std::cout << "Username is too long. Choose another\n";
        Registered();
        return;
    }
    for (int j = 0; j < name.length(); ++j)
    {
        check = ((name[j] >= 'a') && (name[j] <= 'z')) ||
                ((name[j] >= 'A') && (name[j] <= 'Z')) || ((name[j] >= '0') && (name[j] <= '9')) || (name[j] == '_');
        if (check == false)
        {
            std::cout << "Disqualified username. Choose another (consists of upper/lower case letters and/or digits and/or underscore\n";
            Registered();
            return;
        }
    }
    char *nameSend = new char[name.length()];
    char *nameMessage = new char[1];
    for (int i = 0; i < name.length(); ++i)
    {
        nameSend[i] = name[i];
    }
    send(sockfd, nameSend, name.length(), 0);
    // receive message from the server to see if name registered is OK...  0: Already registered name. 1: Success
    recv(sockfd, nameMessage, 1, 0);
    if (nameMessage[0] == 0)
    {
        std::cout << "Already registered username. Choose another\n";
        Registered();
        return;
    }
    else if (nameMessage[0] == 1)
    {
        std::cout << "Successfully assigned username.\n";
        this->username = name;
        this->join = true;
        this->stillQualified = true;
        return;
    }
}
void Play::Skip()
{
    std::cout << "You have chosen to skip this question.\n";
    this->skip = true;
}
void Play::PlayATurn(std::string question, std::string optionA, std::string optionB, std::string optionC, std::string optionD)
{
    char choice;
    std::cout << "Questions: " << question << std::endl;
    std::cout << "Option A: " << optionA << std::endl;
    std::cout << "Option B: " << optionB << std::endl;
    std::cout << "Option C: " << optionC << std::endl;
    std::cout << "Option D: " << optionD << std::endl;
    if (this->skip == false)
    {
        char skip;
        std::cout << "Do you want to skip this question? Remember you are only allowed to skip once!" << std::endl;
        std::cout << "Enter N: No. Enter Y: yes." << std::endl;
        std::cin >> skip;
        if (skip == 'Y')
        {
            Skip();
            // Send message to server to forward the question
            return;
        }
    }
    std::cout << "Your Choice (enter a letter A or a, B or b, C or c, D or d): " << std::endl;
    std::cin >> choice;
    while (choice != 'a' && choice != 'b' && choice != 'c' && choice != 'd' && choice != 'A' && choice != 'B' && choice != 'C' && choice != 'D')
    {
        // Send message to server to forward the question. Send char S.
        char *skipChoice = new char[1];
        char *skipApprove = new char[1];
        skipChoice[0] = 'S';
        send(sockfd, skipChoice, 1, 0);
        Skip();
        return;
    }

    std::cout << "Your Choice (enter a letter A or a, B or b, C or c, D or d): " << std::endl;
    std::cin >> choice;
    while (choice != 'a' && choice != 'b' && choice != 'c' && choice != 'd' && choice != 'A' && choice != 'B' && choice != 'C' && choice != 'D')
    {
        std::cout << "Invalid choice. Choose again." << std::endl;
        std::cin >> choice;
    }
    // Send choice to server, server checks if this user stil qualified, or winner, ...
    char *choiceSend = new char[1];
    char *choiceRcv = new char[1];
    choiceSend[0] = choice;
    send(sockfd, choiceSend, 1, 0);
    // Receive message from server.
    // 0: out of game, 1: in game, 2: winner.
    recv(sockfd, choiceRcv, 1, 0);
    if (choiceRcv[0] == 0)
    {
        this->stillQualified = false;
    }
    else if (choiceRcv[0] == 1)
    {
        // go back to waiting status. Displaying waiting GUI
    }
    else if (choiceRcv[0] == 2)
    {
        // Display congratualation message!
    }
    else
    {
        // Display error message!
    }
}

std::string Play::handleClickEvent(int x, int y)
{
    if (!isMyTurn)
        return "";
    if (x >= posOptA.x && x <= posOptB.x + sizeOpt.x && y >= posOptA.y && y <= posOptC.y + sizeOpt.y)
    {
        if (x <= posOptA.x + sizeOpt.x && y <= posOptA.y + sizeOpt.y)
            return "A";
        else if (x <= posOptC.x + sizeOpt.x && y >= posOptC.y)
            return "C";
        else if (x >= posOptB.x && y <= posOptB.y + sizeOpt.y)
            return "B";
        else if (x >= posOptD.x && y >= posOptD.y)
            return "D";
    }
}

void Play::initPlayers(std::vector<std::string> _usernames)
{
    std::cout << "Play init players\n";
    // usernames.assign(_usernames.begin(), _usernames.end());
    for (std::string each : _usernames)
    {
        usernames.push_back(each);
        Player player(each);
        players.push_back(player);
    }
}

void Play::renderPlayers()
{
    for (int i = 0; i < players.size(); i++)
        players[i].render(window);
}

void Play::renderQuestion()
{
    question.setString(curQuestion.question);
    optA.setString(curQuestion.optionA);
    optB.setString(curQuestion.optionB);
    optC.setString(curQuestion.optionC);
    optD.setString(curQuestion.optionD);
    window->draw(question);
    window->draw(optA);
    window->draw(optB);
    window->draw(optC);
    window->draw(optD);
}

void Play::animatePlayers()
{
    if (clock.getElapsedTime().asSeconds() > 0.02f)
    {
        for (int i = 0; i < players.size(); i++)
        {
            if (players[i].rectSprite.left == players[i].texture.getSize().x - 32)
            {
                players[i].rectSprite.left = 0;
            }
            else
            {
                players[i].rectSprite.left += 32;
            }
            players[i].sprite.setTextureRect(players[i].rectSprite);
        }
        clock.restart();
    }
}

void Play::render()
{
    int x = 2040, initialY = 500;

    for (int i = 0; i < players.size(); i++)
        players[i].initResource(i, sf::Vector2f(x, initialY + i * 180));

    std::string opt = "";
    optChosen = false;

    while (window->isOpen())
    {
        sf::Event event;
        if (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    opt = handleClickEvent(event.mouseButton.x, event.mouseButton.y);
                    if (isMyTurn && opt.length() >= 1) {
                        option = opt;
                        optChosen = true;
                    }
                }
            }
        }
        animatePlayers();
        window->clear();
        window->draw(background);
        renderPlayers();
        renderQuestion();
        window->display();
        if (optChosen) return;
    }
}

void Play::getPlayerAndQuestion(std::string player, Question question)
{
    std::cout << "get player and question\n";
    isMyTurn = (player == username);
    curQuestion = question;
}


std::string Play::getChosenOption() {
    return option;
}