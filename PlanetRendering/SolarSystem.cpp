//
//  SolarSystem.cpp
//  PlanetRendering
//
//  Created by Christian Cosgrove on 10/23/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#include "SolarSystem.h"
#include <iostream>
SolarSystem::SolarSystem(Player& _player, GLManager& _glManager) : player(_player), glManager(_glManager),
    PhysicalSystem(8.,0.01), planets{
        new Planet(glm::vec3(0,-2,0), 1, 100, (vfloat)rand()/RAND_MAX, _player, _glManager),
        new Planet(glm::vec3(0,2, 0), 1, 100, (vfloat)rand()/RAND_MAX, _player, _glManager),
        new Planet(glm::vec3(0,20,0), 1, 100, (vfloat)rand()/RAND_MAX, _player, _glManager)}
{
    for (auto& p : planets) objects.push_back(p);
    planets[1]->Velocity=glm::dvec3(0,0,-10);
    planets[0]->Velocity=glm::dvec3(0,0,10);
    planets[2]->Velocity=glm::dvec3(10,0,0);
    objects.push_back(&player);
}

void SolarSystem::Update()
{
    PhysicalSystem::Update();
}

void SolarSystem::Draw()
{
    for (auto p : planets)
        p->Draw();
}

SolarSystem::~SolarSystem()
{
    for (auto& p : planets) delete p;
}

void SolarSystem::addPlanet(Planet *p)
{
    planets.push_back(p);
    objects.push_back(p);
}

