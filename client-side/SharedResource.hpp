#ifndef SharedResource_hpp
#define SharedResource_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class SharedResource {
    private:
    static SharedResource* instance;
    sf::Font font;

    
    SharedResource();

    public:
    std::vector<sf::Texture> playerTextures;
    SharedResource(SharedResource& obj) = delete;
    static SharedResource* getInstance();
    void initResource();
    sf::Font getFont();
    std::vector<sf::Texture> getTextures();
};



#endif 