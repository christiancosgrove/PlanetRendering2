//
//  SolarSystem.cpp
//  PlanetRendering
//
//  Created by Christian Cosgrove on 10/23/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#include "SolarSystem.h"
#include "RandomUtils.h"
#include "glm/gtc/type_ptr.hpp"
SolarSystem::SolarSystem(Player& _player, GLManager& _glManager, int windowWidth, int windowHeight, const std::string& resourcePath) : player(_player), glManager(_glManager), particleSystem(0),
    PhysicalSystem(8.,0.001, resourcePath), planets{
        new Planet(0,glm::vec3(0,-2,0), 1, 100, RandomUtils::Uniform<vfloat>(-15,25), _player, _glManager, 0.3 + 0*RandomUtils::Uniform<float>(0.05f, 0.8f)),
        new Planet(1,glm::vec3(0,2, 0), 1, 100, RandomUtils::Uniform<vfloat>(-25,25), _player, _glManager, 0.3 + 0*RandomUtils::Uniform<float>(0.05f, 0.8f)),
        new Planet(2,glm::vec3(0,20,0), 1, 100, RandomUtils::Uniform<vfloat>(-10,10), _player, _glManager, 0.3 + 0*RandomUtils::Uniform<float>(0.05f, 0.8f))}
{
#ifdef POSTPROCESSING
    generateRenderTexture(windowWidth,windowHeight);
#endif
    
    particleSystem.AppendParticles(objects);
    for (auto& p : planets)
    {
        objects.push_back(p);
    }
    planets[0]->Velocity=glm::dvec3(0,0,10);
    planets[1]->Velocity=glm::dvec3(0,0,-10);
    planets[2]->Velocity=glm::dvec3(10,0,0);
    objects.push_back(&player);
}

void SolarSystem::Update()
{
    PhysicalSystem::Update();
    particleSystem.Update(TimeStep);
    for (Planet* p:planets)
    {
        for (PhysicsObject* obj:objects)
        {
            if (p==obj || obj==nullptr) continue;
            p->CheckCollision(obj);
        }
    }
    
}

void SolarSystem::Draw(int windowWidth, int windowHeight)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glManager.Programs[3].Use();
    glManager.Programs[3].SetMatrix4("transformMatrix", glm::value_ptr(player.Camera.GetTransformMatrix()));
    particleSystem.Draw();
    glManager.Programs[0].Use();
#ifdef POSTPROCESSING
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0,0,windowWidth,windowHeight);
#endif

    for (auto p : planets)
        p->Draw();
#ifdef POSTPROCESSING
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glManager.Programs[2].Use();
    glBindVertexArray(screenVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
#endif
    
    
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
    glManager.Programs[2].SetTexture("renderedTexture", 0);
    glManager.Programs[2].SetVector2("resolution", glm::vec2(windowWidth,windowHeight));
    glUseProgram(0);
}
void SolarSystem::NextRenderMode()
{
    if (currentRenderMode==Planet::RenderMode::WIRE) currentRenderMode=Planet::RenderMode::SOLID;
    else currentRenderMode=Planet::RenderMode::WIRE;
    for (Planet* p : planets)
    {
        p->CurrentRenderMode=currentRenderMode;
    }
}