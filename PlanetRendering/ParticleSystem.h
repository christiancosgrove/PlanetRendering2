//
//  ParticleSystem.h
//  PlanetRendering
//
//  Created by Christian Cosgrove on 1/30/15.
//  Copyright (c) 2015 Christian. All rights reserved.
//
#pragma once
#include "PhysicsObject.h"
#include <vector>
#include "glm/glm.hpp"
struct Particle : PhysicsObject
{
    bool Inactive = true;
    Particle(glm::dvec3 position, glm::dvec3 velocity, double mass);
};


class ParticleSystem
{
public:
    
    const int NUM_PARTICLES;
    
    
    ParticleSystem();
private:
    
    int currParticle;
    
    std::vector<Particle> particles = std::vector<Particle>(NUM_PARTICLES);
    
    inline void AddParticle(const Particle& particle);
};