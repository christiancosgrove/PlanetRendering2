//
//  MainGame_SDL.cpp
//  PlanetRendering
//
//  Created by Christian on 10/1/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#include "MainGame_SDL.h"
#include "ResourcePath.hpp"
#include <stdexcept>
#include "GLManager.h"
#include "Planet.h"
#include <iostream>
MainGame_SDL::MainGame_SDL() : gameState(GameState::PLAY)
{
    if (SDL_Init(SDL_INIT_VIDEO)) throw std::logic_error("Failed to initialize SDL!  " + std::string(SDL_GetError()));
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetSwapInterval(1);
    
    window = SDL_CreateWindow("Planet Rendering", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window==nullptr) throw std::logic_error("Window failed to be initialized");
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context==nullptr) throw std::logic_error("SDL_GL could not be initialized!");
    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    GLManager glManager(resourcePath() + "fragmentShader.glsl", resourcePath() + "vertexShader.glsl");
    glManager.Program.Use();
    glClearColor(0,0,0,1);
    
    std::cout << "gl_renderer:" << glGetString(GL_RENDERER) << std::endl;
    std::cout << "gl_context: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "gl_shading_lang_version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    Player player(WINDOW_WIDTH, WINDOW_HEIGHT);
    Planet planet(glm::vec3(0,0,0), 1);
    
    
    while (gameState!=GameState::EXIT)
    {
        HandleEvents();
        Draw(planet, player,glManager);
        Update(planet, player);
    }
    
}

void MainGame_SDL::Draw(Planet& planet, Player& player, GLManager& glManager)
{
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    planet.Draw(player, glManager);
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