#include <SFML/Graphics.hpp>
#include <iostream>

#include "SharedResource.hpp"
#include "Register.hpp"
SharedResource *SharedResource::instance = nullptr; 

int main()
{
    sf::RenderWindow window(sf::VideoMode(2560, 1440), "Who wants to be millionaire?");

    Register rgt(&window);   

    // rgt.render();
    sf::Texture texture;
    texture.loadFromFile("../client-side/assets/img/sprites/frog-idle.png");
    sf::IntRect rectSprite(0, 0, 32, 32);
    sf::Sprite sprite; //(texture, rectSprite);
    sprite.setTexture(texture);
    sprite.setTextureRect(rectSprite);
    sprite.setPosition(500, 500);
    sprite.scale(sf::Vector2f(3, 3));

    // rgt.background.setTexture(texture);
    while (window.isOpen()) {
        rgt.updatePollEvents();
        window.clear();
        rgt.render();
        // window.draw(sprite);
        
        window.display();
    }


    return 0;
}

void ingamescreen() {
    sf::RenderWindow window(sf::VideoMode(2560, 1440), "SFML works!");

    // SharedResource* sharedResource = SharedResource::getInstance();

    std::vector<std::string> usernames;

    // variables 
    int players_pos_x = 2560 - 640; // region left x
    int players_pos_y = 0;
    int avt_pos_x = 2560 - 640 + 24; // region left x  + 24
    int avt_pos_y = 300;
    int username_pos_x = 2560 - 640 + 24 + 96 + 12; // avt left x
    int username_pos_y = 300 + 36 ;// avt top y + 36

    // avatar
    sf::Texture texture;
    texture.loadFromFile("../client-side/assets/img/sprites/frog-idle.png");
    sf::IntRect rectSprite(0, 0, 32, 32);
    sf::Sprite sprite; //(texture, rectSprite);
    sprite.setTexture(texture);
    sprite.setTextureRect(rectSprite);
    sprite.setPosition(avt_pos_x, avt_pos_y);
    sprite.scale(sf::Vector2f(3, 3));

    // username
    sf::Font font;
    if (!font.loadFromFile("../client-side/assets/font/Poppins-Medium.ttf")) std::cout << "Error loading font\n";
    sf::Text text("username", font, 24);
    text.setFillColor(sf::Color::Black);
    text.setPosition(username_pos_x, username_pos_y);

    // background for player list
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(640, 1440));
    rect.setPosition(players_pos_x, players_pos_y);
    rect.setFillColor(sf::Color::White);

    sf::Clock clock;
    int change = 32;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (clock.getElapsedTime().asSeconds() > 0.02f)
        {
            if (rectSprite.left >= 320)
                change = -32;
            if (rectSprite.left == 0) change = 32;
            rectSprite.left += change;
            sprite.setTextureRect(rectSprite);
            clock.restart();
        }

        window.clear();
        window.draw(rect);
        window.draw(sprite);
        window.draw(text);
        window.display();
    }
}


// g++ sfml.cpp -I/opt/homebrew/Cellar/sfml/2.5.1_2 -o client -L/opt/homebrew/Cellar/sfml/2.5.1_2/lib -lsfml-graphics -lsfml-window -lsfml-system
