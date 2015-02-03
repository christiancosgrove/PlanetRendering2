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
#include "ParticleSystem.h"
class SolarSystem : public PhysicalSystem
{
public:
    SolarSystem(Player& _player, GLManager& _glManager, int windowWidth, int windowHeight, const std::string& resourcePath);
    ~SolarSystem();
    void Update();
    void Draw(int windowWidth, int windowHeight);
    void NextRenderMode();
private:
    Player& player;
    GLManager& glManager;
    std::vector<Planet*> planets;
    void addPlanet(Planet* p);
    GLuint framebuffer;
    GLuint screenVBO;
    GLuint screenVAO;
    GLuint texLocation;
    Planet::RenderMode currentRenderMode;
    
    //temp
    ParticleSystem particleSystem;
    
    void generateRenderTexture(int windowWidth, int windowHeight);
};