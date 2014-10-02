//
//  MainGame_SDL.h
//  PlanetRendering
//
//  Created by Christian on 10/1/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#ifndef __PlanetRendering__MainGame_SDL__
#define __PlanetRendering__MainGame_SDL__

#include <SDL2/SDL.h>
#include <OpenGL/gl.h>
#include "Planet.h"
#include "Player.h"


class MainGame_SDL
{
public:
    
    enum class GameState
    {
        PLAY, EXIT,
    };
    static const int WINDOW_WIDTH = 800;
    static const int WINDOW_HEIGHT = 600;
    MainGame_SDL();
    void Draw(Planet& planet, Player& player);
    void Update(Planet& planet, Player& player);
    void HandleEvents();
private:
    SDL_Window* window;
    GameState gameState;
};

#endif /* defined(__PlanetRendering__MainGame_SDL__) */
