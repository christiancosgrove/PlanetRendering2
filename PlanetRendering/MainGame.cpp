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

MainGame::MainGame() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Planet Rendering", sf::Style::Default, sf::ContextSettings(0,0,0,3,3))
{
    std::cout << "version " << window.getSettings().majorVersion << "." << window.getSettings().minorVersion << std::endl;
    GLManager glManager(resourcePath() + "fragmentShader.glsl", resourcePath() + "vertexShader.glsl");
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
                
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);
        window.display();
    }
    
}
