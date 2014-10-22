//
//  PlanetAtmosphere.cpp
//  PlanetRendering
//
//  Created by Christian Cosgrove on 10/21/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#include "PlanetAtmosphere.h"

PlanetAtmosphere::PlanetAtmosphere(glm::vec3 position, float radius) : Position(position), RADIUS(radius)
{
    generateBuffers();
    buildMesh();
}



void PlanetAtmosphere::generateBuffers()
{
    glGenVertexArrays(1, &sphereVAO);
    glBindVertexArray(sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glGenBuffers(1, &sphereIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereIBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindVertexArray(0);
}

void PlanetAtmosphere::buildMesh()
{
    const int azimuthalVertices = 512;
    const int polarVertices = 512;
    for (float i = 0; i<2 * M_PI; i+=2*M_PI / azimuthalVertices)
    {
        for (float j = 0; j<2 * M_PI; j+=2*M_PI / polarVertices)
        {
            unsigned int currIndex = vertices.size();
            vertices.push_back(Vertex(RADIUS * glm::vec3(std::cos(i) * std::sin(j), std::sin(i) * std::sin(j), cos (j))));
            vertices.push_back(Vertex(RADIUS * glm::vec3(std::cos(i) * std::sin(j+2*M_PI / polarVertices), std::sin(i) * std::sin(j+2*M_PI / polarVertices), cos (j+2*M_PI / polarVertices))));
            vertices.push_back(Vertex(RADIUS * glm::vec3(std::cos(i+2*M_PI / azimuthalVertices) * std::sin(j), std::sin(i+2*M_PI / azimuthalVertices) * std::sin(j), cos (j))));
            vertices.push_back(Vertex(RADIUS * glm::vec3(std::cos(i+2*M_PI / azimuthalVertices) * std::sin(j+2*M_PI / polarVertices), std::sin(i+2*M_PI / azimuthalVertices) * std::sin(j+2*M_PI / polarVertices), cos (j+2*M_PI / polarVertices))));
            
            indices.push_back(currIndex + 2);
            indices.push_back(currIndex + 1);
            indices.push_back(currIndex + 0);
            
            indices.push_back(currIndex + 2);
            indices.push_back(currIndex + 3);
            indices.push_back(currIndex + 1);
        }
    }
    glBindVertexArray(sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PlanetAtmosphere::Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, sphereIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

void PlanetAtmosphere::Draw()
{
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}
