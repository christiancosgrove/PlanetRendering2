//
//  PlanetAtmosphere.cpp
//  PlanetRendering
//
//  Created by Christian Cosgrove on 10/21/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#include "PlanetAtmosphere.h"
#include "GLManager.h"
#include "Planet.h"
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
            size_t currIndex = vertices.size();
            vertices.push_back(Vertex(RADIUS * glm::vec3(std::cos(i) * std::sin(j), std::sin(i) * std::sin(j), cos (j))));
            vertices.push_back(Vertex(RADIUS * glm::vec3(std::cos(i) * std::sin(j+2*M_PI / polarVertices), std::sin(i) * std::sin(j+2*M_PI / polarVertices), cos (j+2*M_PI / polarVertices))));
            vertices.push_back(Vertex(RADIUS * glm::vec3(std::cos(i+2*M_PI / azimuthalVertices) * std::sin(j), std::sin(i+2*M_PI / azimuthalVertices) * std::sin(j), cos (j))));
            vertices.push_back(Vertex(RADIUS * glm::vec3(std::cos(i+2*M_PI / azimuthalVertices) * std::sin(j+2*M_PI / polarVertices), std::sin(i+2*M_PI / azimuthalVertices) * std::sin(j+2*M_PI / polarVertices), cos (j+2*M_PI / polarVertices))));
            
            indices.push_back((unsigned)(currIndex + 2));
            indices.push_back((unsigned)(currIndex + 1));
            indices.push_back((unsigned)(currIndex + 0));
            
            indices.push_back((unsigned)(currIndex + 2));
            indices.push_back((unsigned)(currIndex + 3));
            indices.push_back((unsigned)(currIndex + 1));
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
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}
void PlanetAtmosphere::SetUniforms(GLManager& glManager, Planet& planet)
{
    glManager.Programs[1].Use();
//    uniform vec3 v3CameraPos;		// The camera's current position
//    uniform vec3 v3LightPos = vec3(0,1,0);		// The direction vector to the light source
//    uniform vec3 v3InvWavelength = vec3(100.,100.,1000.);	// 1 / pow(wavelength, 4) for the red, green, and blue channels
    glManager.Programs[1].SetVector3("v3InvWavelength", glm::vec3(0.18867780436772762, 0.4978442963618773, 0.6616065586417131
));
//    uniform float fCameraHeight;	// The camera's current height
//    uniform float fCameraHeight2;	// fCameraHeight^2
//    uniform float fOuterRadius = 1.01;		// The outer (atmosphere) radius
    glManager.Programs[1].SetFloat("fOuterRadius", RADIUS);
    //    uniform float fOuterRadius2 = 1.01*1.01;	// fOuterRadius^2
    glManager.Programs[1].SetFloat("fOuterRadius2", RADIUS*RADIUS);
    //    uniform float fInnerRadius = 1.;		// The inner (planetary) radius
    glManager.Programs[1].SetFloat("fInnerRadius", planet.Radius);
    //    uniform float fInnerRadius2 = 1.;	// fInnerRadius^2
    glManager.Programs[1].SetFloat("fInnerRadius2", planet.Radius*planet.Radius);
    float fKrESun = 0.025f;
    float fKmESun = 0.025f;
    //    uniform float fKrESun = 10.;			// Kr * ESun
    glManager.Programs[1].SetFloat("fKrESun", fKrESun);
    //    uniform float fKmESun = 10.;			// Km * ESun
    glManager.Programs[1].SetFloat("fKmESun", fKmESun);
    glManager.Programs[1].SetFloat("fKr4PI", fKrESun * 4 * M_PI);
    glManager.Programs[1].SetFloat("fKm4PI", fKrESun * 4 * M_PI);
    float fScale =1./(RADIUS - planet.Radius);
    float fScaleDepth =1.+(RADIUS - planet.Radius)/2.;
    glManager.Programs[1].SetFloat("fScale", fScale);
    glManager.Programs[1].SetFloat("fScaleDepth", fScaleDepth);
    glManager.Programs[1].SetFloat("fScaleOverScaleDepth", fScale/fScaleDepth);
    
//    uniform float fScale = 1./(1.001-1.);			// 1 / (fOuterRadius - fInnerRadius)
//    uniform float fScaleDepth = 1.005;		// The scale depth (i.e. the altitude at which the atmosphere's average density is found)
//    uniform float fScaleOverScaleDepth = 10./1.005;	// fScale / fScaleDepth
//    uniform vec3	lightDir = vec3(0,-1,0);
//    uniform vec3	earthCenter = vec3(0,0,0);
    

}
