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
        particles[i].Mass = std::numeric_limits<double>::epsilon();
        particles[i].Energy=0;
        particles[i].Inactive=false;
        particles[i].Position = glm::vec3(
                                          RandomUtils::Normal<float>(0, 1),
                                          RandomUtils::Normal<float>(0, 1),
                                          RandomUtils::Normal<float>(0, 1));
        particles[i].Velocity = glm::vec3(
                                          RandomUtils::Normal<float>(0, 0.5),
                                          RandomUtils::Normal<float>(0, 0.5),
                                          RandomUtils::Normal<float>(0, 0.5));
    }
    glBindVertexArray(vao);
    updateVBO();
    glBindVertexArray(0);
}

void ParticleSystem::generateVBO()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    glBindVertexArray(0);
    printf("GL error1: %i\n", glGetError());
}

void ParticleSystem::updateVBO()
{
    for (int i = 0; i<NUM_PARTICLES;i++)
    {
        drawArray[i] = particles[i].Position;
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * drawArray.size(), &drawArray[0], GL_DYNAMIC_DRAW);
}

void ParticleSystem::Draw()
{
    glPointSize(5);
    //todo: fix: this drawing method causes crashes.
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    updateVBO();
    glDrawArrays(GL_POINTS, 0, (GLsizei)drawArray.size());
    glBindVertexArray(0);
    
}

void ParticleSystem::Update(double timeStep)
{
    for (auto& p:particles)
    {
//        p.ApplyForce(glm::dvec3(0,0,1));
        p.UpdatePhysics(timeStep);
    }
}

void ParticleSystem::AppendParticles(std::vector<PhysicsObject *> &physicsObjects)
{
    for (int i = 0; i<NUM_PARTICLES;i++)
    {
        physicsObjects.push_back(&particles[i]);
    }
}

