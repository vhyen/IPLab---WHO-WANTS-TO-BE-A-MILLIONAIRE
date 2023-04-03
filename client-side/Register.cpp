#include "Register.hpp"

Register::Register()
{
    window = new sf::RenderWindow(sf::VideoMode(2560, 1440), "Who is millionaire?");
    initResource();
}

Register::Register(sf::RenderWindow *window)
{
    this->window = window;
    initResource();
}

Register::~Register()
{
}

void Register::initResource()
{
    username = "";
    // font
    font.loadFromFile("../client-side/assets/font/Poppins-Medium.ttf");
    // background
    if (!texture.loadFromFile("../client-side/assets/img/backgrounds/register-background.png"))
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
            break;
        }
        if (event.type == sf::Event::TextEntered)
        {
            if ((event.text.unicode >= 48 && event.text.unicode <= 57) || (event.text.unicode >= 65 && event.text.unicode <= 90) || event.text.unicode == 95 || (event.text.unicode >= 97 && event.text.unicode <= 122))
            {
                username += event.text.unicode;
                // mFilename += event.text.unicode;
                playerInput.setString(username);
                std::cout << username << std::endl;
            }
            if (event.text.unicode == 8)
                username.pop_back();
                playerInput.setString(username);
        }
    }
}

void Register::render()
{
    // window->clear();

    window->draw(background);
    window->draw(playerInput);
    // window->draw(playerInput);
    // window->draw(displayNoti);
    // window->display();
}

std::string Register::getUsername()
{
    return username;
}

bool Register::validateUsername()
{
    return true;
}