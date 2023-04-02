#include <SFML/Graphics.hpp>
#include <iostream>

#include "SharedResource.hpp"

SharedResource *SharedResource::instance = nullptr;

int main()
{
    sf::RenderWindow window(sf::VideoMode(2560, 1440), "SFML works!");

    // SharedResource* sharedResource = SharedResource::getInstance();

    sf::Texture texture;
    texture.loadFromFile("../client-side/assets/img/sprites/frog-idle.png");
    sf::IntRect rectSprite(0, 0, 32, 32);
    sf::Sprite sprite(texture, rectSprite);
    sprite.setPosition(300, 300);
    sprite.scale(sf::Vector2f(4, 4));

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
        window.draw(sprite);
        window.display();
    }

    return 0;
}

// g++ sfml.cpp -I/opt/homebrew/Cellar/sfml/2.5.1_2 -o client -L/opt/homebrew/Cellar/sfml/2.5.1_2/lib -lsfml-graphics -lsfml-window -lsfml-system
