//
//  ParticleSystem.cpp
//  PlanetRendering
//
//  Created by Christian Cosgrove on 1/30/15.
//  Copyright (c) 2015 Christian. All rights reserved.
//

#include "ParticleSystem.h"

#include "RandomUtils.h"

Particle::Particle(glm::dvec3 position, glm::dvec3 velocity, double mass) : PhysicsObject(position, velocity, mass) {}


ParticleSystem::ParticleSystem(int numParticles) : NUM_PARTICLES(numParticles), particles(NUM_PARTICLES), drawArray(NUM_PARTICLES)
{
    generateVBO();
    
    for (int i = 0; i<NUM_PARTICLES;i++)
    {
        particles[i].Position = glm::vec3(
                                          RandomUtils::Normal<float>(0, 0.1),
                                          RandomUtils::Normal<float>(0, 0.1),
                                          RandomUtils::Normal<float>(0, 0.1));
    }
}

void ParticleSystem::generateVBO()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    
    glBindVertexArray(0);
    printf("GL error1: %i\n", glGetError());
}

void ParticleSystem::updateVBO()
{
    for (int i = 0; i<NUM_PARTICLES;i++)
    {
        drawArray[i] = glm::vec3(particles[i].Position);
    }
    glBindVertexArray(vao);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * drawArray.size(), &drawArray[0], GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
}

void ParticleSystem::Draw()
{
    updateVBO();
    
    //todo: fix: this drawing method causes crashes.
    glBindVertexArray(vao);

    glDrawArrays(GL_POINTS, 0, drawArray.size());
    
    glBindVertexArray(0);
    
}

void ParticleSystem::Update(double timeStep)
{
//    for (Particle& p:particles)
//    {
//        
//        p.UpdatePhysics(timeStep);
//    }
}

