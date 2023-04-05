#include "Register.hpp"

Register::Register()
{
    window = new sf::RenderWindow(sf::VideoMode(2560, 1440), "Who is millionaire?");
    isDone = false;
    isNoti = false;
    initResource();
}

Register::Register(sf::RenderWindow *window)
{
    this->window = window;
    isDone = false;
    initResource();
}

Register::~Register()
{
}

void Register::initResource()
{
    username = "";
    // font
    font.loadFromFile("../client-side/assets/fonts/Poppins-Medium.ttf");
    // background
    if (!texture.loadFromFile("../client-side/assets/images/backgrounds/register-background.png"))
        std::cout << "cannot load texture\n";
    background.setTexture(texture);
    background.scale(sf::Vector2f(1.34, 1.34));
    background.setPosition(0, 0);
    // text  input
    playerInput.setFont(font);
    playerInput.setCharacterSize(52);
    playerInput.setColor(sf::Color::White);
    playerInput.setPosition(1000, 800);
    // display notification
    displayNoti.setFont(font);
    displayNoti.setCharacterSize(40);
    displayNoti.setColor(sf::Color::Yellow);
    displayNoti.setPosition(780, 1200);
    displayNoti.setString("");
}

void Register::updatePollEvents()
{
    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.Event::type == sf::Event::Closed)
        {
            // handling close events (including closing socket ... )
            window->close();
        }
        if (event.type == sf::Event::TextEntered)
        {
            if ((event.text.unicode >= 48 && event.text.unicode <= 57) // 0 - 9
                || (event.text.unicode >= 65 && event.text.unicode <= 90)  // A - Z
                || event.text.unicode == 95 
                || (event.text.unicode >= 97 && event.text.unicode <= 122)) // a - z
            {
                if (username.size() == 10) { // handle maximum characters
                    displayNoti.setString("maximum 10 characters");
                }
                else {
                    username += event.text.unicode;
                    playerInput.setString(username);
                    displayNoti.setString("");
                }
            }
            else if (event.text.unicode == 8) {
                if (username.length() != 0) username.pop_back();
                playerInput.setString(username);
                displayNoti.setString("");
            }
            else if (event.text.unicode == 46) { // cannot hit enter in mac :v so hit . to enter
                isDone = true;
                std::cout << "Enter hit\n";
            }
            else {
                // handle inputting other stuffs
                displayNoti.setString("only accept letters, digits and underscores");
            }
        }
    }
}

void Register::render()
{
    while (window->isOpen() && !isDone) {
        updatePollEvents();
        window->clear();
        window->draw(background);
        window->draw(playerInput);
        window->draw(displayNoti);
        window->display();
    }
}

std::string Register::getUsername()
{
    return username;
}

bool Register::validateUsername() // send to server and waiting for respond
{
    return true;
}

bool Register::isDoneRegister() {
    return isDone;
}