//
//  ParticleSystem.h
//  PlanetRendering
//
//  Created by Christian Cosgrove on 1/30/15.
//  Copyright (c) 2015 Christian. All rights reserved.
//
#pragma once
#include <OpenGL/gl3.h>
#include "PhysicsObject.h"
#include <vector>
#include "glm/glm.hpp"
struct Particle : PhysicsObject
{
    bool Inactive = true;
    Particle(glm::dvec3 position, glm::dvec3 velocity, double mass);
    Particle() : PhysicsObject(glm::dvec3(), glm::dvec3(), 0.0) {}
};


class ParticleSystem
{
public:
    
    const int NUM_PARTICLES;
    
    
    ParticleSystem(int numParticles);
    inline void AddParticle(const Particle& particle);
    void Update(double timeStep);
    void Draw();
    
private:
    
    void generateVBO();
    
    void updateVBO();
    
    GLuint vao, vbo;
    std::vector<glm::vec3> drawArray;
    
    int currParticle;
    
    std::vector<Particle> particles;
    
};

void ParticleSystem::AddParticle(const Particle &particle)
{
    for (int i = 0; i<NUM_PARTICLES && !particles[currParticle].Inactive; i++)
    {
        currParticle++;
        currParticle%=NUM_PARTICLES;
    }
    particles[currParticle] = particle;
}