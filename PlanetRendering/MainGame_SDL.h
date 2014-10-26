//
//  MainGame_SDL.h
//  PlanetRendering
//
//  Created by Christian on 10/1/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//
#pragma once 

#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>
#include "Planet.h"
#include "Player.h"
#include "SolarSystem.h"
//this class encapsulates all functionality of the of the program
class MainGame_SDL
{
public:
    ///abstraction used to detect when game is closed (extendable to other UI features)
    enum class GameState
    {
        PLAY, EXIT,
    };
    static const int WINDOW_WIDTH = 1200;
    static const int WINDOW_HEIGHT = 900;
    ///Initialization of SDL Window, GL, and planet
    MainGame_SDL();
    ///All rendering takes place in here
    void Draw(SolarSystem& solarSystem, Player& player, GLManager& glManager);
    ///All terrain subdivision and player movement here
    void Update(SolarSystem& solarSystem, Player& player);
    ///handles keyboard events -- e.g. close window
    void HandleEvents(SolarSystem& solarSystem);
    static vfloat ElapsedMilliseconds;
private:
    ///SDL window for program
    SDL_Window* window;
    ///current game state
    GameState gameState;
};
