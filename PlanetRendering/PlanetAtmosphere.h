//
//  PlanetAtmosphere.h
//  PlanetRendering
//
//  Created by Christian Cosgrove on 10/21/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#pragma once
#include <OpenGL/gl3.h>
#include "glm/glm.hpp"
#include <vector>
#include "PhysicsObject.h"

class GLManager;
class Planet;
class PlanetAtmosphere
{
public:
    struct Vertex
    {
        glm::vec3 position;
        Vertex(float x, float y, float z) : position(x,y,z) {}
        Vertex(glm::vec3 pos) : position(pos) {}
    };
    PlanetAtmosphere(glm::vec3 position, float radius);
    glm::vec3 Position;
    const float RADIUS;
    void Draw();
    void SetUniforms(GLManager& glManager, Planet& planet);
private:
    GLuint sphereVBO;
    GLuint sphereIBO;
    GLuint sphereVAO;
    std::vector<unsigned int> indices;
    std::vector<Vertex> vertices;
    void buildMesh();
    void generateBuffers();
};