#pragma once 

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Register {
    private:
    //window
    bool isDone;
    sf::RenderWindow* window;
    // resources
    sf::Font font;
    sf::Texture texture;
    sf::Sprite background;
    
    sf::Text playerInput, displayNoti;
    bool isNoti;
    std::string username;

    void initResource();
    bool validateUsername();

    public:
    Register();
    Register(sf::RenderWindow* window);
    virtual ~Register();
    void updatePollEvents();
    void render();
    std::string getUsername();
    bool isDoneRegister();
};