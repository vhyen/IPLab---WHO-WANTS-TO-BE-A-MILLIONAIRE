#ifndef SharedResource_hpp
#define SharedResource_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>

class SharedResource {
    private:
    static SharedResource* instance;
    sf::Font font;
    
    SharedResource();

    public:
    static SharedResource* getInstance();
    
    void initResource();
    sf::Font getFont();
};



#endif 