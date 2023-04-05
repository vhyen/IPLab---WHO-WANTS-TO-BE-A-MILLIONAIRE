#include "SharedResource.hpp"

SharedResource::SharedResource() {
    
}

SharedResource* SharedResource::getInstance() {
    if (!instance) {
        instance = new SharedResource();
    }
    return instance;
}

void SharedResource::initResource() {
    std::cout << "SharedResource::initResource\n";
    if (!font.loadFromFile("../client-side/assets/fonts/Poppins-Medium.ttf")) {
        std::cerr << "Error loading font\n";
    }
    for (int i = 1; i <= 6; i++) { // 6 png for player avt
        sf::Texture texture;
        texture.loadFromFile("../client-side/assets/images/sprites/" + std::to_string(i) +  ".png");
        playerTextures.push_back(texture);
    }
    std::cout << playerTextures.size() << std::endl;
}

sf::Font SharedResource::getFont() {
    return font;
}

std::vector<sf::Texture> SharedResource::getTextures() {
    std::cout << "SharedResource::getTexture\n";
    return playerTextures;
}