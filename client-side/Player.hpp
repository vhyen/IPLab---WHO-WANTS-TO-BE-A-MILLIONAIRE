#ifndef Player_hpp
#define Player_hpp

#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "SharedResource.hpp"

class Player {
    public:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::IntRect rectSprite;
    sf::Text displayUsername;
    std::string username;
    sf::Font font;

    Player(std::string);
    Player(std::string _username, sf::Vector2f pos);
    void initResource(int playerInd, sf::Vector2f pos);
    void render(sf::RenderWindow* window);
    void info();
};

#endif