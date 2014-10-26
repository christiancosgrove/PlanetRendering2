//
//  SolarSystem.cpp
//  PlanetRendering
//
//  Created by Christian Cosgrove on 10/23/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#include "SolarSystem.h"
#include <iostream>
SolarSystem::SolarSystem(Player& _player, GLManager& _glManager) : player(_player), glManager(_glManager), currTrackTargetIndex(-1), PhysicalSystem(6.73e-10,0.001), planet(glm::vec3(0,0,0), 1, (vfloat)rand()/RAND_MAX, _player, _glManager), planet2(glm::vec3(0,20,0), 1, (vfloat)rand()/RAND_MAX, _player, _glManager)
{
//    addPlanet(new Planet(glm::vec3(0,0,0), 1, (vfloat)rand()/RAND_MAX, _player, _glManager));
////    addPlanet(new Planet(glm::vec3(-3.,0,0), 1, (vfloat)rand()/RAND_MAX, player, glManager));
    addPlanet(&planet);
    addPlanet(&planet2);
    planet2.Velocity=glm::dvec3(10.,-5.,0.0);
    planet.Velocity=glm::dvec3(-10.,5.,0.0);
    objects.push_back(&player);
//    player.Velocity.z = 0.;
}

void SolarSystem::Update()
{
    auto currPos = planet.Position;
    PhysicalSystem::Update();
    std::cout << "POS: " << objects[2]->Position.x << std::endl;
//    if (currTrackTargetIndex==0)
//    {
//        player.Camera.position-=planet.Position-currPos;
//    }
}

void SolarSystem::Draw()
{
//    for (Planet* p : planets)
    ////        p->Draw();
    for (auto p : planets)
        p->Draw();
}

SolarSystem::~SolarSystem()
{
}

void SolarSystem::addPlanet(Planet *p)
{
    planets.push_back(p);
    objects.push_back(p);
}



void SolarSystem::NextTrackTarget()
{
    currTrackTargetIndex++;
    if (currTrackTargetIndex-1 > planets.size())
        currTrackTargetIndex=-1;
//    if (currTrackTargetIndex==0) player.Camera.position+=planet.Position;
//    else player.Camera.position-=planet.Position;
}
void SolarSystem::PrevTrackTarget()
{
    currTrackTargetIndex--;
    if (currTrackTargetIndex<-1)
        currTrackTargetIndex=planets.size()-1;
//    if (currTrackTargetIndex==0) player.Camera.position+=planet.Position;
//    else player.Camera.position-=planet.Position;
}
