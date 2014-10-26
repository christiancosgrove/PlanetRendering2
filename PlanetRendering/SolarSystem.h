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
class SolarSystem : public PhysicalSystem
{
public:
    SolarSystem(Player& _player, GLManager& _glManager);
    ~SolarSystem();
    void Update();
    void Draw();
    void NextTrackTarget();
    void PrevTrackTarget();
private:
    Player& player;
    GLManager& glManager;
    int currTrackTargetIndex;
    Planet planet;
    Planet planet2;
    std::vector<Planet*> planets;
//    std::vector<Planet*> planets;
    void addPlanet(Planet* p);
};