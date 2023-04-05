#include "Player.hpp"

Player::Player(std::string _username) {
    username = _username;
}

Player::Player(std::string _username, sf::Vector2f pos)
{
    SharedResource* sr = SharedResource::getInstance();
    std::cout << "Player constructor\n";
    sprite.setPosition(pos);
    username = _username;
}

void Player::render(sf::RenderWindow *window)
{
    window->draw(sprite);
    window->draw(displayUsername);
}

void Player::info()
{
    std::cout << "texture: " << texture.getSize().x << " " << texture.getSize().y << std::endl;
    std::cout << "sprite: " << sprite.getTexture()
              << " " << sprite.getPosition().x << " " << sprite.getPosition().y << " " << sprite.getTextureRect().height << std::endl;
}

void Player::initResource(int playerInd, sf::Vector2f pos) {
    // username
    int scale = 4;
    int fontsize = 8*scale, diffx = 32*scale + 32, diffy = 12*scale;
    if (!font.loadFromFile("../client-side/assets/fonts/Poppins-Medium.ttf")) std::cout << "Error loading font\n";
    displayUsername.setFont(font);
    displayUsername.setCharacterSize(fontsize);
    displayUsername.setString(username);
    displayUsername.setPosition(pos.x + 32*scale + 32, pos.y + 12*scale);
    displayUsername.setFillColor(sf::Color::White);
    // sprite
    texture.loadFromFile("../client-side/assets/images/sprites/" + std::to_string(playerInd) +  ".png");
    rectSprite.left = rectSprite.top = 0;
    rectSprite.width = rectSprite.height = texture.getSize().y;
    sprite.setTexture(texture);
    sprite.setTextureRect(rectSprite);
    sprite.setPosition(pos);
    sprite.scale(sf::Vector2f(scale, scale));
}