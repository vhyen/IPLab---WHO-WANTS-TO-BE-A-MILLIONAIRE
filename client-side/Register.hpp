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
    sf::Texture texture, scrWaiting;
    sf::Sprite background;
    
    sf::Text playerInput, displayNoti;
    bool isNoti;
    std::string username;

    void initResource();

    public:
    Register();
    Register(sf::RenderWindow* window);
    virtual ~Register();
    void updatePollEvents();
    void existedUsername();
    void render();
    void renderWaiting();
    std::string getUsername();
    bool isDoneRegister();
    void reset();
};