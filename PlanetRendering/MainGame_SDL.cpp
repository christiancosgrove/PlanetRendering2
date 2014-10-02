//
//  MainGame_SDL.cpp
//  PlanetRendering
//
//  Created by Christian on 10/1/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#include "MainGame_SDL.h"
#include <stdexcept>
#include "GLManager.h"
#include "Planet.h"
MainGame_SDL::MainGame_SDL() : gameState(GameState::PLAY)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    
    window = SDL_CreateWindow("Planet Rendering", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if (window==nullptr) throw std::logic_error("Window failed to be initialized");
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context==nullptr) throw std::logic_error("SDL_GL could not be initialized!");
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    glClearColor(0,1,0,1);
    
    
    //GLManager glManager(resourcePath() + "fragmentShader.glsl", resourcePath() + "vertexShader.glsl");
    Player player(WINDOW_WIDTH, WINDOW_HEIGHT);
    Planet planet(glm::vec3(0,0,0), 1);
    
    
    while (gameState!=GameState::EXIT)
    {
        HandleEvents();
        Draw(planet, player);
        Update(planet, player);
    }
    
}

void MainGame_SDL::Draw(Planet& planet, Player& player)
{
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    planet.Draw(player);
    SDL_GL_SwapWindow(window);
}

void MainGame_SDL::Update(Planet& planet, Player& player)
{
    planet.Update(player);
    player.Update();
}

void MainGame_SDL::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                gameState = GameState::EXIT;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
            {
                case SDL_SCANCODE_ESCAPE:
                    gameState = GameState::EXIT;
                    break;
            }
                break;
        }
    }
}