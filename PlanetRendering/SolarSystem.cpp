//
//  SolarSystem.cpp
//  PlanetRendering
//
//  Created by Christian Cosgrove on 10/23/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#include "SolarSystem.h"
#include "RandomUtils.h"
SolarSystem::SolarSystem(Player& _player, GLManager& _glManager, int windowWidth, int windowHeight) : player(_player), glManager(_glManager),
    PhysicalSystem(8.,0.001), planets{
        new Planet(glm::vec3(0,-2,0), 1, 100, RandomUtils::Uniform<vfloat>(-1e2f,1e2f), _player, _glManager, RandomUtils::Uniform<float>(0.05f, 0.8f)),
        new Planet(glm::vec3(0,2, 0), 1, 100, RandomUtils::Uniform<vfloat>(-1e2f,1e2f), _player, _glManager, RandomUtils::Uniform<float>(0.05f, 0.8f)),
        new Planet(glm::vec3(0,20,0), 1, 100, RandomUtils::Uniform<vfloat>(-1e2f,1e2f), _player, _glManager, RandomUtils::Uniform<float>(0.05f, 0.8f))}
{
    generateRenderTexture(windowWidth,windowHeight);
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

#include <iostream>
void SolarSystem::Draw(int windowWidth, int windowHeight)
{
    glManager.Programs[0].Use();
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,windowWidth,windowHeight);

    for (auto p : planets)
        p->Draw();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glManager.Programs[2].Use();
    glBindVertexArray(screenVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

SolarSystem::~SolarSystem()
{
    glDeleteFramebuffers(1, &framebuffer);
    for (auto& p : planets) delete p;
}

void SolarSystem::addPlanet(Planet *p)
{
    planets.push_back(p);
    objects.push_back(p);
}
static const GLfloat g_quad_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    1.0f,  1.0f, 0.0f,
};
void SolarSystem::generateRenderTexture(int windowWidth, int windowHeight)
{
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    GLuint texture;
    GLuint renderBuffer;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    //TODO: make this resolution agnostic
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth,windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth,windowHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    texLocation = glGetUniformLocation( glManager.Programs[2].programID, "renderedTexture");
    
    glGenVertexArrays(1, &screenVAO);
    glBindVertexArray(screenVAO);
    
    glGenBuffers(1, &screenVBO);
    glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
    
    glBindVertexArray(0);
    
    glManager.Programs[2].Use();
    glManager.Programs[2].SetVector2("resolution", glm::vec2(windowWidth,windowHeight));
    glUseProgram(0);
}