#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "SharedResource.hpp"
#include "Question.hpp"  

struct Player {
    std::string username;
    sf::Texture texture;
    sf::Sprite sprite;
};

class Game {
    private: 
    // window
    sf::RenderWindow *window;
    // resources
    SharedResource* sharedResource = SharedResource::getInstance();
    sf::Font font;
    sf::Sprite clock;
    sf::Text question, answerA, answerB, answerC, answerD;
    sf::Text curQuestNum, totalQuestNum;
    // buttons ?

    void initResources();
    void initPlayers();
    sf::Sprite inGameBackground, endGameBackground;

    public: 
    Game();
    virtual ~Game();
    void updatePlayers();
    void updatePollEvents();
    void updateQuestion(Question& question);
    void updateBackground();
    void render();
};