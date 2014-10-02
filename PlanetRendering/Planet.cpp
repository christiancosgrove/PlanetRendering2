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

Planet::Planet(glm::vec3 pos, float radius) : Position(pos), Radius(radius)
{
    buildBaseMesh();
    generateBuffers();
}

bool Planet::trySubdivide(std::vector<Face>::iterator& iterator, const std::function<bool (Player&, Face)>& func, Player& player, std::vector<Face>& newFaces)
{
    
    Face f(*iterator);
    
    if (func(player, f))
    {
        glm::vec3 m12 = Normalize((f.v1 + f.v2) * 0.5f)*Radius;
        glm::vec3 m13 = Normalize((f.v1 + f.v3) * 0.5f)*Radius;
        glm::vec3 m23 = Normalize((f.v2 + f.v3) * 0.5f)*Radius;
        
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
        if (trySubdivide(it, [](Player& player, Face f)->bool { return f.level < 3; }, player, newFaces)) wasSubdivided=true;
    }
    for (Face f : newFaces) faces.push_back(f);
    if (wasSubdivided) updateVBO();
}

void Planet::generateBuffers()
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    updateVBO();
}

void Planet::updateVBO()
{
//    vertices.clear();
//    for (Face f : faces)
//    {
//        vertices.push_back(Vertex(f.v1));
//        vertices.push_back(Vertex(f.v1));
//        vertices.push_back(Vertex(f.v1));
//    }
    
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
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
    
    
    
//    
//    float phi = (1 + sqrt(5))/2;
//    glm::vec3 c0 = glm::vec3(0, 1, phi);
//    glm::vec3 c1 = glm::vec3(0, -1, phi);
//    glm::vec3 c2 = glm::vec3(0, 1, -phi);
//    glm::vec3 c3 = glm::vec3(0, -1, -phi);
//    
//    glm::vec3 c4 = glm::vec3(1, phi, 0);
//    glm::vec3 c5 = glm::vec3(-1, phi, 0);
//    glm::vec3 c6 = glm::vec3(1, -phi, 0);
//    glm::vec3 c7 = glm::vec3(-1, -phi, 0);
//    
//    glm::vec3 c8 = glm::vec3(phi, 0, 1);
//    glm::vec3 c9 = glm::vec3(phi, 0, 1);
//    glm::vec3 c10 = glm::vec3(phi, 0, 1);
//    glm::vec3 c11 = glm::vec3(phi, 0, 1);
//    
//    faces.push_back(<#const_reference __x#>)
    
    
    //faces.push_back(Face(glm::vec3(0,0,1),glm::vec3(1,0,0),glm::vec3(0,1,0)));
}

void Planet::Draw(Player& player)
{
//    glEnableClientState(GL_VERTEX_ARRAY);
//    //glEnableVertexAttribArray(0);
//    
//    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glVertexPointer(3, GL_FLOAT, 0, &faces[0]);
//    glDrawArrays(GL_TRIANGLES, 0, faces.size()*3);
//    
//    //glDisableVertexAttribArray(0);
//    glDisableClientState(GL_VERTEX_ARRAY);
    
    glLoadMatrixf(glm::value_ptr(player.Camera.GetTransformMatrix()));
    
    //TODO: fix to use vertex arrays instead -- this is incredibly inefficient
    for (auto f : faces)
    {
        glBegin(GL_TRIANGLES);
        glColor3f(1.0,0.0,0.0);
        glVertex3f(f.v1.x,f.v1.y,f.v1.z);
        glColor3f(0,0.0,1.0);
        glVertex3f(f.v2.x,f.v2.y,f.v2.z);
        glColor3f(0,1.0,0.0);
        glVertex3f(f.v3.x,f.v3.y,f.v3.z);
        glEnd();
    }
}