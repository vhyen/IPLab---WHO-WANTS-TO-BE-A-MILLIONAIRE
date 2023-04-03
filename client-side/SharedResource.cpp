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
    if (!font.loadFromFile("../client-side/assets/font/Poppins-Medium.ttf")) {
        std::cerr << "Error loading font\n";
    }
}

sf::Font SharedResource::getFont() {
    return font;
}