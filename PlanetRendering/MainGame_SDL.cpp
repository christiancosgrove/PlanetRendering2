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

vfloat MainGame_SDL::ElapsedMilliseconds = 0.0f;

MainGame_SDL::MainGame_SDL() : gameState(GameState::PLAY)
{
    //if SDL fails, close program
    if (SDL_Init(SDL_INIT_VIDEO)) throw std::logic_error("Failed to initialize SDL!  " + std::string(SDL_GetError()));
    
    /* SDL Configuration */
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetSwapInterval(1);
    
    
    //Initialize window and check for errors
    window = SDL_CreateWindow("Planet Rendering", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    if (window==nullptr) throw std::logic_error("Window failed to be initialized");
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context==nullptr) throw std::logic_error("SDL_GL could not be initialized!");
    
    
    //Initialize GLManager object - provides OOP abstraction of some OpenGL API features (shader programs)
    GLManager glManager(resourcePath() + "fragmentShader.glsl", resourcePath() + "vertexShader.glsl");
    glManager.Program.Use();
    
    //Set background color and default depth buffer values
    glClearColor(0,0,0,1);
    glClearDepth(1.0);
    
    
    //print OpenGL version for reference
    std::cout << "gl_renderer:" << glGetString(GL_RENDERER) << std::endl;
    std::cout << "gl_context: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "gl_shading_lang_version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);
    //initialize player object (contains camera functionality)
    Player player(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    //seed random generator (change time(nullptr) to number for a deterministic seed)
    srand(time(nullptr));
    Planet planet(glm::vec3(0,0,0), 1, (vfloat)rand()/RAND_MAX, player, glManager);
    std::cout << "GL error: " << glGetError() << std::endl;
    
    //Main loop
    while (gameState!=GameState::EXIT)
    {
        HandleEvents(planet);
        Draw(planet, player,glManager);
        Update(planet, player);
    }
    
}

void MainGame_SDL::Draw(Planet& planet, Player& player, GLManager& glManager)
{
    clock_t now = clock();
    //clear color & depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    planet.Draw(player, glManager);
    
    //swap doublebuffers (doublebuffering prevents screen tearing)
    SDL_GL_SwapWindow(window);
    ElapsedMilliseconds = (vfloat)(clock_t() - now) / CLOCKS_PER_SEC;
}

void MainGame_SDL::Update(Planet& planet, Player& player)
{
    //aplanet.Update(player);
    player.Update();
}

void MainGame_SDL::HandleEvents(Planet& planet)
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
                case SDL_SCANCODE_TAB:
                if (planet.CurrentRenderMode==Planet::RenderMode::SOLID) planet.CurrentRenderMode=Planet::RenderMode::WIRE;
                else planet.CurrentRenderMode=Planet::RenderMode::SOLID;
                break;
            }
                break;
        }
    }
}