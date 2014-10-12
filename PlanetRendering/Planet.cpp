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
#include <algorithm>
#include <random>



//Constructor for planet.  Initializes VBO (experimental) and builds the base icosahedron mesh.
Planet::Planet(glm::vec3 pos, float radius) : Position(pos), Radius(radius), time(0)
{
    
    generateBuffers();
    buildBaseMesh();
}

Planet::~Planet()
{
    glDeleteVertexArrays(1, &VAO);
}

//This function accepts a boolean-valued function of displacement.  If the function is true, the function will divide the given face into four subfaces, each of which will be recursively subjected to the same subdivision scheme.  It is important that the input function terminates at a particular level of detail, or the program will crash.
bool Planet::trySubdivide(Face* iterator, const std::function<bool (Player&, const Face&)>& func, Player& player)
{
    
    
    if (func(player, *iterator))
    {
        if (iterator->child0!=nullptr || iterator->child1!=nullptr || iterator->child2!=nullptr || iterator->child3!=nullptr)
            return true;
        
        
        
        glm::dvec3 v1 = iterator->v1;
        glm::dvec3 v2 = iterator->v2;
        glm::dvec3 v3 = iterator->v3;
        
        glm::dvec3 nv1 = glm::normalize(v1);
        glm::dvec3 nv2 = glm::normalize(v2);
        glm::dvec3 nv3 = glm::normalize(v3);
        
        double l1 = glm::length(v1);
        double l2 = glm::length(v2);
        double l3 = glm::length(v3);
        
        
        glm::dvec3 m12 = glm::normalize((nv1 + nv2) * 0.5)*Radius;
        glm::dvec3 m13 = glm::normalize((nv1 + nv3) * 0.5)*Radius;
        glm::dvec3 m23 = glm::normalize((nv2 + nv3) * 0.5)*Radius;
        
    
        
        
        
        double fac =1./(double)(1 << iterator->level);
        
        
        m12*=1 + terrainNoise(m12) * fac;
        m13*=1 + terrainNoise(m13) * fac;
        m23*=1 + terrainNoise(m23) * fac;
        
        m12*=(l1 + l2)/2.;
        m13*=(l1 + l3)/2.;
        m23*=(l2 + l3)/2.;
        
        
        Face* f0 = new Face(m13,m12,m23, iterator->level+1);
        Face* f1 = new Face(v3,m13,m23,iterator->level+1);
        Face* f2 = new Face(m23,m12,v2,iterator->level+1);
        Face* f3 = new Face(m13,v1,m12,iterator->level+1);
        
        //iterator=faces.erase(iterator);
//        newFaces.push_back(f0);
//        newFaces.push_back(f1);
//        newFaces.push_back(f2);
        //        newFaces.push_back(f3);
        iterator->child0 = f0;
        iterator->child1 = f1;
        iterator->child2 = f2;
        iterator->child3 = f3;
        
        return true;
    }
//    else { ++iterator; return false; }
    return false;
}
//Similarly to trySubdivide, this function combines four faces into a larger face if a boolean-valued function is statisfied.
bool Planet::tryCombine(Face* iterator, const std::function<bool (Player&, const Face&)>& func, Player& player)
{
    
    //TODO: check for shared vertices.  If vertices are shared, combine four triangles.
    if (iterator==nullptr) return false;
    if (iterator->child0==nullptr || iterator->child1==nullptr || iterator->child2==nullptr || iterator->child3==nullptr) return false;
    if ((func(player, *iterator) || func(player, *iterator->child0)|| func(player, *iterator->child1) || func(player, *iterator->child2) || func(player, *iterator->child3)) && iterator->level>0)
    {
        combineFace(iterator);
        
        return true;
    }
    return false;
}

void Planet::combineFace(Face* face)
{
    if (face->level==0) return;
    if (face->child0!=nullptr)
    {
        combineFace(face->child0);
        delete face->child0;
        face->child0 = nullptr;
    }
    if (face->child1!=nullptr)
    {
        combineFace(face->child1);
        delete face->child1;
        face->child1 = nullptr;
    }
    if (face->child2!=nullptr)
    {
        combineFace(face->child2);
        delete face->child2;
        face->child2 = nullptr;
    }
    if (face->child3!=nullptr)
    {
        combineFace(face->child3);
        delete face->child3;
        face->child3 = nullptr;
    }
}

void Planet::Update(Player& player)
{
    bool wasSubdivided = false;
    if (time%10==0)
    {
        for (auto it = faces.begin();it!=faces.end();it++)
        {
            if (recursiveCombine(&(*it), player))
                wasSubdivided=true;
            if (recursiveSubdivide(&(*it), player))
                wasSubdivided=true;
        }
    }
    if (wasSubdivided || vertices.size()==0) updateVBO();
}

void Planet::generateBuffers()
{
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glEnableVertexAttribArray(0);
    glVertexAttribLPointer(0, 4, GL_DOUBLE, 0, (void*)0);
    glBindVertexArray(0);
//    updateVBO();
}

void Planet::recursiveUpdate(Face& face)
{
    if (face.child0!=nullptr && face.child1!=nullptr && face.child2!=nullptr && face.child3!=nullptr)
    {
        recursiveUpdate(*face.child0);
        recursiveUpdate(*face.child1);
        recursiveUpdate(*face.child2);
        recursiveUpdate(*face.child3);
    }
    else
    {
        vertices.push_back(Vertex(glm::dvec4(face.v1,1.0f)));
        vertices.push_back(Vertex(glm::dvec4(face.v2,1.0f)));
        vertices.push_back(Vertex(glm::dvec4(face.v3,1.0f)));
    }
}


bool Planet::recursiveSubdivide(Face* face, Player& player)
{
    if (trySubdivide(face,
                     [this](Player& player, Face f)->bool {
                         return std::max(std::max(
                                                  glm::length(-player.Camera.Position - f.v1),
                                                  glm::length(-player.Camera.Position - f.v2)),
                                         glm::length(-player.Camera.Position - f.v3))
                         < (float)(1 << LOD_MULTIPLIER) / ((float)(1 << (f.level))); }
                     , player))
    {
        recursiveSubdivide(face->child0,player);
        recursiveSubdivide(face->child1,player);
        recursiveSubdivide(face->child2,player);
        recursiveSubdivide(face->child3,player);
        return true;
    }
    return false;
}

bool Planet::recursiveCombine(Face* face, Player& player)
{
    if (face==nullptr) return false;
    if (!tryCombine(face,
                    
                    [this](Player& player, Face f)->bool { return std::min(std::min(
                                                                                    glm::length(-player.Camera.Position - f.v1),
                                                                                    glm::length(-player.Camera.Position - f.v2)),
                                                                           glm::length(-player.Camera.Position - f.v3))
                        >= (double)(1 << (LOD_MULTIPLIER)) / ((double)(1 << (f.level-1))); }
                    
                    , player))
    {
        
        recursiveCombine(face->child0, player) ||
                recursiveCombine(face->child1, player) ||
                recursiveCombine(face->child2, player) ||
        recursiveCombine(face->child3, player);
        return false;
    }
    else return true;
}

void Planet::updateVBO()
{
    vertices.clear();
    for (Face& f : faces)
        recursiveUpdate(f);
    
    if (vertices.size()>0)
    {
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    }
}


void Planet::buildBaseMesh()
{
    glm::dvec3 icosahedron[12];
    
    double theta = 26.56505117707799 * M_PI / 180.0; // refer paper for theta value
    
    double stheta = std::sin(theta);
    double ctheta = std::cos(theta);
    
    icosahedron[0] = glm::dvec3(0.0f, 0.0f, -1.0f); // bottom vertex of icosahedron
    
    // the lower pentagon
    double phi = M_PI / 5.0;
    for (int i = 1; i < 6; ++i) {
        icosahedron[i] = glm::dvec3(
                                    ctheta * std::cos(phi), ctheta * std::sin(phi), -stheta);
        
        phi += 2.0 * M_PI / 5.0;
    }
    
    // the upper pentagon
    phi = 0.0;
    for (int i = 6; i < 11; ++i) {
        icosahedron[i] = glm::dvec3(
                                          ctheta * std::cos(phi), ctheta * std::sin(phi), stheta);
        
        phi += 2.0 * M_PI / 5.0;
    }
    
    icosahedron[11] = glm::dvec3(0.0, 0.0, 1.0); // top vertex of icosahedron
    
    faces.push_back(Face(icosahedron[0], icosahedron[2], icosahedron[1]));
    faces.push_back(Face(icosahedron[0], icosahedron[3], icosahedron[2]));
    faces.push_back(Face(icosahedron[0], icosahedron[4], icosahedron[3]));
    faces.push_back(Face(icosahedron[0], icosahedron[5], icosahedron[4]));
    faces.push_back(Face(icosahedron[0], icosahedron[1], icosahedron[5]));
    
    faces.push_back(Face(icosahedron[1], icosahedron[2], icosahedron[7]));
    faces.push_back(Face(icosahedron[2], icosahedron[3], icosahedron[8]));
    faces.push_back(Face(icosahedron[3], icosahedron[4], icosahedron[9]));
    faces.push_back(Face(icosahedron[4], icosahedron[5], icosahedron[10]));
    faces.push_back(Face(icosahedron[5], icosahedron[1], icosahedron[6]));
    
    faces.push_back(Face(icosahedron[1], icosahedron[7], icosahedron[6]));
    faces.push_back(Face(icosahedron[2], icosahedron[8], icosahedron[7]));
    faces.push_back(Face(icosahedron[3], icosahedron[9], icosahedron[8]));
    faces.push_back(Face(icosahedron[4], icosahedron[10], icosahedron[9]));
    faces.push_back(Face(icosahedron[5], icosahedron[6], icosahedron[10]));
    
    faces.push_back(Face(icosahedron[6], icosahedron[7], icosahedron[11]));
    faces.push_back(Face(icosahedron[7], icosahedron[8], icosahedron[11]));
    faces.push_back(Face(icosahedron[8], icosahedron[9], icosahedron[11]));
    faces.push_back(Face(icosahedron[9], icosahedron[10], icosahedron[11]));
    faces.push_back(Face(icosahedron[10], icosahedron[6], icosahedron[11]));
    
}

void Planet::Draw(Player& player, GLManager& glManager)
{
//    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glManager.Program.Use();
    glUniform1f(1,(GLfloat)time);
    
    glManager.Program.SetMatrix4dv("transformMatrix", glm::value_ptr(player.Camera.GetTransformMatrix()));
    
    
    
    if (vertices.size() >0)
    {
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glBindVertexArray(0);
    }
}