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
Planet::Planet(glm::vec3 pos, float radius) : Position(pos), Radius(radius)
{
    generateBuffers();
    buildBaseMesh();
}

bool Planet::trySubdivide(std::vector<Face>::iterator& iterator, const std::function<bool (Player&, Face)>& func, Player& player, std::vector<Face>& newFaces)
{
    
    Face f(*iterator);
    
    if (func(player, f))
    {
        glm::vec3 m12 = Normalize((f.v1 + f.v2) * 0.5f)*Radius;
        glm::vec3 m13 = Normalize((f.v1 + f.v3) * 0.5f)*Radius;
        glm::vec3 m23 = Normalize((f.v2 + f.v3) * 0.5f)*Radius;
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
bool Planet::tryCombine(std::vector<Face>::iterator& iterator, const std::function<bool (Player&, Face)>& func, Player& player, std::vector<Face>& newFaces)
{
    
    Face f(*iterator);
    
    if (func(player, f))
    {
        glm::vec3 m12 = Normalize((f.v1 + f.v2) * 0.5f)*Radius;
        glm::vec3 m13 = Normalize((f.v1 + f.v3) * 0.5f)*Radius;
        glm::vec3 m23 = Normalize((f.v2 + f.v3) * 0.5f)*Radius;
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

void Planet::Update(Player& player)
{
    bool wasSubdivided = false;
    std::vector<Face> newFaces;
    for (auto it = faces.begin();it!=faces.end();)
    {
        if (trySubdivide(it, [](Player& player, Face f)->bool { return std::min(std::min(
                                                                                glm::length(-player.Camera.Position - f.v1),
                                                                                glm::length(-player.Camera.Position - f.v2)),
                                                                                glm::length(-player.Camera.Position - f.v3)) < exp2(-(float)f.level+4); }, player, newFaces)) wasSubdivided=true;
    }
    for (Face f : newFaces) faces.push_back(f);
    if (wasSubdivided && vertices.size()==0) updateVBO();
}

void Planet::generateBuffers()
{
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 0.0f,1.0f,
    0.0f,  1.0f, 0.0f,1.0f,
};

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
//    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
//
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    std::cout << "error: " << (glGetError()) << std::endl;
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
        glLoadMatrixf(glm::value_ptr(player.Camera.GetTransformMatrix()));
    GLuint attribLoc = glGetAttribLocation(glManager.Program.programID, "vertexPos");
    
//    if (vertices.size() >0)
//    {
//        glBindBuffer(GL_ARRAY_BUFFER, VBO);
//        
////        glBindVertexArrayAPPLE(VAO);
//        
//        //glVertexPointer(4, GL_FLOAT, 0, (void*)0);
//        glVertexAttribPointer(attribLoc, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//        
//        glDisableVertexAttribArray(0);
//    }
//    
    //TODO: fix to use vertex arrays instead -- this is incredibly inefficient
    for (auto f : faces)
    {
        glBegin(GL_LINES);
        glColor3f(1.0,0.0,0.0);
        glVertex3f(f.v1.x,f.v1.y,f.v1.z);
        glColor3f(0,0.0,1.0);
        glVertex3f(f.v2.x,f.v2.y,f.v2.z);
        glColor3f(0,1.0,0.0);
        
        glVertex3f(f.v1.x,f.v1.y,f.v1.z);
        glVertex3f(f.v3.x,f.v3.y,f.v3.z);
        glVertex3f(f.v2.x,f.v2.y,f.v2.z);
        glVertex3f(f.v3.x,f.v3.y,f.v3.z);
        
        
        glEnd();
    }
}