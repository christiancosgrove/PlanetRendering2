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
#include <OpenGL/gl3.h>
#include "Planet.h"
#include "Player.h"

//this class encapsulates all functionality of the of the program
class MainGame_SDL
{
public:
    //abstraction used to detect when game is closed (extendable to other UI features)
    enum class GameState
    {
        PLAY, EXIT,
    };
    static const int WINDOW_WIDTH = 1200;
    static const int WINDOW_HEIGHT = 900;
    //Initialization of SDL Window, GL, and planet
    MainGame_SDL();
    //All rendering takes place in here
    void Draw(Planet& planet, Player& player, GLManager& glManager);
    //All terrain subdivision and player movement here
    void Update(Planet& planet, Player& player);
    //handles keyboard events -- e.g. close window
    void HandleEvents(Planet& planet);
private:
    //SDL window for program
    SDL_Window* window;
    //current game state
    GameState gameState;
};

#endif /* defined(__PlanetRendering__MainGame_SDL__) */
