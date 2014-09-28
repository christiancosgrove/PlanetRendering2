//
//  MainGame.h
//  PlanetRendering
//
//  Created by Christian on 9/28/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#ifndef __PlanetRendering__MainGame__
#define __PlanetRendering__MainGame__

#include <SFML/Window.hpp>

class MainGame
{
public:
    static const int WINDOW_WIDTH = 800;
    static const int WINDOW_HEIGHT = 600;
    MainGame();
private:
    sf::Window window;
};



#endif /* defined(__PlanetRendering__MainGame__) */
