//
//  SolarSystem.h
//  PlanetRendering
//
//  Created by Christian Cosgrove on 10/23/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//
#pragma once
#include "PhysicalSystem.h"
#include "Planet.h"
#include <array>
class SolarSystem : public PhysicalSystem
{
public:
    SolarSystem(Player& _player, GLManager& _glManager);
    ~SolarSystem();
    void Update();
    void Draw();
private:
    Player& player;
    GLManager& glManager;
    std::vector<Planet*> planets;
    void addPlanet(Planet* p);
    GLuint framebuffer;
    GLuint screenVBO;
    GLuint screenVAO;
    GLuint texLocation;
    void generateRenderTexture();
};