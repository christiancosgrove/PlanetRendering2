//
//  MainGame.cpp
//  PlanetRendering
//
//  Created by Christian on 9/28/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#include "MainGame.h"
#include <SFML/OpenGL.hpp>
#include "ResourcePath.hpp"
#include "GLManager.h"
#include <iostream>
#include "Planet.h"

MainGame::MainGame() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Planet Rendering", sf::Style::Default, sf::ContextSettings(0,0,0,3,3))
{
    std::cout << "version " << window.getSettings().majorVersion << "." << window.getSettings().minorVersion << std::endl;
    //GLManager glManager(resourcePath() + "fragmentShader.glsl", resourcePath() + "vertexShader.glsl");
    Player player(WINDOW_WIDTH, WINDOW_HEIGHT);
    Planet planet(glm::vec3(0,0,0), 1);
    window.setFramerateLimit(60);
    bool open = true;
    while (open)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    open = false;
                    break;
                case sf::Event::KeyPressed:
                    switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        open = false;
                }
                    break;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);
        planet.Update(player);
        planet.Draw(player);
        player.Update();
        window.display();
    }
    
}
