#include "Register.hpp"

Register::Register()
{
    window = new sf::RenderWindow(sf::VideoMode(2560, 1440), "Who is millionaire?");
    isDone = false;
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            std::cout << username << std::endl;
            isDone = true;
            std::cout << "Enter\n";
            window->clear();
            // break;
        }
        if (event.type == sf::Event::TextEntered)
        {
            if ((event.text.unicode >= 48 && event.text.unicode <= 57) // 0 - 9
                || (event.text.unicode >= 65 && event.text.unicode <= 90)  // A - Z
                || event.text.unicode == 95 
                || (event.text.unicode >= 97 && event.text.unicode <= 122)) // a - z
            {
                username += event.text.unicode;
                playerInput.setString(username);
            }
            if (event.text.unicode == 8) {
                if (username.length() != 0) username.pop_back();
                playerInput.setString(username);
            }
            if (event.text.unicode == 46) { // cannot hit enter in mac :v so hit . to enter
                isDone = true;
                std::cout << "Enter hit\n";
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