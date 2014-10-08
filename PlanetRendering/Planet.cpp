//
//  Planet.cpp
//  PlanetRendering
//
//  Created by Christian on 9/28/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#include "Planet.h"
#include "VectorHelper.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

//Constructor for planet.  Initializes VBO (experimental) and builds the base icosahedron mesh.
Planet::Planet(glm::vec3 pos, float radius) : Position(pos), Radius(radius)
{
    generateBuffers();
    buildBaseMesh();
}

Planet::~Planet()
{
    glDeleteVertexArrays(1, &VAO);
}

//This function accepts a boolean-valued function of displacement.  If the function is true, the function will divide the given face into four subfaces, each of which will be recursively subjected to the same subdivision scheme.  It is important that the input function terminates at a particular level of detail, or the program will crash.
bool Planet::trySubdivide(std::vector<Face>::iterator& iterator, const std::function<bool (Player&, Face)>& func, Player& player, std::vector<Face>& newFaces)
{
    
    Face f(*iterator);
    
    if (func(player, f))
    {
        glm::vec3 m12 = Normalize((f.v1 + f.v2) * 0.5f)*Radius;
        glm::vec3 m13 = Normalize((f.v1 + f.v3) * 0.5f)*Radius;
        glm::vec3 m23 = Normalize((f.v2 + f.v3) * 0.5f)*Radius;
        //float off =0.1f * randFloat() / (float)(1<<f.level);
        m12*=1 + terrainNoise(m12);
        m13*=1 + terrainNoise(m13);
        m23*=1 + terrainNoise(m23);
        
        Face f1(m13,m23,m12, f.level+1);
        Face f2(f.v1,m13,m12,f.level+1);
        Face f3(m12,m23,f.v2,f.level+1);
        Face f4(m13,f.v3,m23,f.level+1);
        
        iterator=faces.erase(iterator);
        newFaces.push_back(f1);
        newFaces.push_back(f2);
        newFaces.push_back(f3);
        newFaces.push_back(f4);
        return true;
    }
    else { ++iterator; return false; }
}
//Similarly to trySubdivide, this function combines four faces into a larger face if a boolean-valued function is statisfied.
bool Planet::tryCombine(std::vector<Face>::iterator& iterator, const std::function<bool (Player&, Face)>& func, Player& player, std::vector<Face>& newFaces)
{
    
    Face f(*iterator);
    Face f1,f2,f3;
    //TODO: check for shared vertices.  If vertices are shared, combine four triangles.
    
    if ((iterator+1)!=faces.end())
        if ((iterator+1)->level==f.level) f1 = *(iterator+1);
    else return false;
    else return false;
    if ((iterator+2)!=faces.end())
        if ((iterator+2)->level==f.level) f2 = *(iterator+2);
    else return false;
    else return false;
    if ((iterator+3)!=faces.end())
        if ((iterator+3)->level==f.level) f3 = *(iterator+3);
    else return false;
    else return false;
    
    if (func(player, f) || func(player, f1)|| func(player, f2) || func(player, f3))
    {
        for (int i = 0; i<4 && iterator!=faces.end();i++) iterator = faces.erase(iterator);
        Face nf(f.v1,f2.v3,f3.v2);
        newFaces.push_back(nf);
        return true;
    }
    else { ++iterator; return false; }
}

void Planet::Update(Player& player)
{
    bool wasSubdivided = false;
    std::vector<Face> newFaces;
    for (auto it = faces.begin();it!=faces.end();)
    {
        
//        if (tryCombine(it, [this](Player& player, Face f)->bool { return std::min(std::min(
//                                                                                                glm::length(-player.Camera.Position - f.v1),
//                                                                                                glm::length(-player.Camera.Position - f.v2)),
//                                                                                       glm::length(-player.Camera.Position - f.v3)) > (float)(1 << (LOD_MULTIPLIER+2)) / ((float)(1 << (f.level))); }, player, newFaces)) wasSubdivided=true;
         if (trySubdivide(it, [this](Player& player, Face f)->bool { return std::min(std::min(
                                                                                         glm::length(-player.Camera.Position - f.v1),
                                                                                         glm::length(-player.Camera.Position - f.v2)),
                                                                                glm::length(-player.Camera.Position - f.v3)) < (float)(1 << LOD_MULTIPLIER) / ((float)(1 << (f.level))); }, player, newFaces)) wasSubdivided=true;
        
    }
    for (Face f : newFaces) faces.push_back(f);
    if (wasSubdivided || vertices.size()==0) updateVBO();
}

void Planet::generateBuffers()
{
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindVertexArray(0);
    updateVBO();
}

void Planet::updateVBO()
{
    vertices.clear();
    for (Face f : faces)
    {
        vertices.push_back(Vertex(glm::vec4(f.v1,1.0f)));
        vertices.push_back(Vertex(glm::vec4(f.v2,1.0f)));
        vertices.push_back(Vertex(glm::vec4(f.v3,1.0f)));
    }
    std::cout << vertices.size()<<std::endl;
    
    
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
}


void Planet::buildBaseMesh()
{
    glm::vec3 icosaVertices[12];
    
    double theta = 26.56505117707799f * M_PI / 180.0; // refer paper for theta value
    
    double stheta = std::sin(theta);
    double ctheta = std::cos(theta);
    
    icosaVertices[0] = glm::vec3(0.0f, 0.0f, -1.0f); // the lower vertex
    
    // the lower pentagon
    double phi = M_PI / 5.0;
    for (int i = 1; i < 6; ++i) {
        icosaVertices[i] = glm::vec3(
                                    ctheta * std::cos(phi), ctheta * std::sin(phi), -stheta);
        
        phi += 2.0 * M_PI / 5.0;
    }
    
    // the upper pentagon
    phi = 0.0;
    for (int i = 6; i < 11; ++i) {
        icosaVertices[i] = glm::vec3(
                                          ctheta * std::cos(phi), ctheta * std::sin(phi), stheta);
        
        phi += 2.0 * M_PI / 5.0;
    }
    
    icosaVertices[11] = glm::vec3(0.0f, 0.0f, 1.0f); // the upper vertex
    
    faces.push_back(Face(icosaVertices[0], icosaVertices[3], icosaVertices[2]));
    faces.push_back(Face(icosaVertices[0], icosaVertices[4], icosaVertices[3]));
    faces.push_back(Face(icosaVertices[0], icosaVertices[5], icosaVertices[4]));
    faces.push_back(Face(icosaVertices[0], icosaVertices[1], icosaVertices[5]));
    
    faces.push_back(Face(icosaVertices[1], icosaVertices[2], icosaVertices[7]));
    faces.push_back(Face(icosaVertices[2], icosaVertices[3], icosaVertices[8]));
    faces.push_back(Face(icosaVertices[3], icosaVertices[4], icosaVertices[9]));
    faces.push_back(Face(icosaVertices[4], icosaVertices[5], icosaVertices[10]));
    faces.push_back(Face(icosaVertices[5], icosaVertices[1], icosaVertices[6]));
    
    faces.push_back(Face(icosaVertices[1], icosaVertices[7], icosaVertices[6]));
    faces.push_back(Face(icosaVertices[2], icosaVertices[8], icosaVertices[7]));
    faces.push_back(Face(icosaVertices[3], icosaVertices[9], icosaVertices[8]));
    faces.push_back(Face(icosaVertices[4], icosaVertices[10], icosaVertices[9]));
    faces.push_back(Face(icosaVertices[5], icosaVertices[6], icosaVertices[10]));
    
    faces.push_back(Face(icosaVertices[6], icosaVertices[7], icosaVertices[11]));
    faces.push_back(Face(icosaVertices[7], icosaVertices[8], icosaVertices[11]));
    faces.push_back(Face(icosaVertices[8], icosaVertices[9], icosaVertices[11]));
    faces.push_back(Face(icosaVertices[9], icosaVertices[10], icosaVertices[11]));
    faces.push_back(Face(icosaVertices[10], icosaVertices[6], icosaVertices[11]));
    
}

void Planet::Draw(Player& player, GLManager& glManager)
{
    glManager.Program.SetMatrix4fv("transformMatrix", glm::value_ptr(player.Camera.GetTransformMatrix()));

    if (vertices.size() >0)
    {
        
        glBindVertexArray(VAO);
        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        //glDisableVertexAttribArray(0);
        glBindVertexArray(0);
    }
}