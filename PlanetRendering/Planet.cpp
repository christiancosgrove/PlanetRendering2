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
bool Planet::trySubdivide(Face* iterator, const std::function<bool (Player&, const Face&)>& func, Player& player)
{
    
    //Face f(f_old.v3,f_old.v2,f_old.v1,f_old.level);
    
    if (func(player, *iterator))
    {
        glm::vec3 m12 = Normalize((iterator->v1 + iterator->v2) * 0.5f)*Radius;
        glm::vec3 m13 = Normalize((iterator->v1 + iterator->v3) * 0.5f)*Radius;
        glm::vec3 m23 = Normalize((iterator->v2 + iterator->v3) * 0.5f)*Radius;
        //float off =0.1f * randFloat() / (float)(1<<f.level);
        m12*=1 + terrainNoise(m12);
        m13*=1 + terrainNoise(m13);
        m23*=1 + terrainNoise(m23);
        
        if (iterator->child0==nullptr && iterator->child1==nullptr && iterator->child2==nullptr && iterator->child3==nullptr)
        {
        Face* f0 = new Face(m13,m12,m23, iterator->level+1);
        Face* f1 = new Face(iterator->v3,m13,m23,iterator->level+1);
        Face* f2 = new Face(m23,m12,iterator->v2,iterator->level+1);
        Face* f3 = new Face(m13,iterator->v1,m12,iterator->level+1);
        
        //iterator=faces.erase(iterator);
//        newFaces.push_back(f0);
//        newFaces.push_back(f1);
//        newFaces.push_back(f2);
        //        newFaces.push_back(f3);
        iterator->child0 = f0;
        iterator->child1 = f1;
        iterator->child2 = f2;
        iterator->child3 = f3;
        }
        return true;
    }
//    else { ++iterator; return false; }
    return false;
}
//Similarly to trySubdivide, this function combines four faces into a larger face if a boolean-valued function is statisfied.
bool Planet::tryCombine(Face* iterator, const std::function<bool (Player&, const Face&)>& func, Player& player)
{
    
    //TODO: check for shared vertices.  If vertices are shared, combine four triangles.
//    
//    Face f1,f2,f3;
    
    
//    auto it1 = std::find_if(faces.begin(), faces.end(), [f](const Face& face) -> bool{ return face.v3==f.v3 && face.level==f.level; });
//    if (it1==faces.end()) return false;
//    auto it2 = std::find_if(faces.begin(), faces.end(), [f](const Face& face) -> bool{ return face.v2==f.v2 && face.level==f.level; });
//    if (it2==faces.end()) return false;
//    auto it3 = std::find_if(faces.begin(), faces.end(), [f](const Face& face) -> bool{ return face.v1==f.v1 && face.level==f.level; });
//    if (it3==faces.end()) return false;
//    
//    Face f1(*it1);
//    Face f2(*it2);
//    Face f3(*it3);
    
//    if ((iterator+1)!=faces.end())
//        if ((iterator+1)->level==f.level && (iterator+1)->v3==f.v3) f1 = *(iterator+1);
//    else return false;
//    else return false;
//    if ((iterator+2)!=faces.end())
//        if ((iterator+2)->level==f.level && (iterator+2)->v2==f.v2) f2 = *(iterator+2);
//    else return false;
//    else return false;
//    if ((iterator+3)!=faces.end())
//        if ((iterator+3)->level==f.level && (iterator+3)->v1==f.v1) f3 = *(iterator+3);
//    else return false;
//    else return false;
    
    if ((func(player, *iterator) || func(player, *iterator->child0)|| func(player, *iterator->child1) || func(player, *iterator->child2) || func(player, *iterator->child3)) && iterator->level>0)
    {
//        for (int i = 0; i<4 && iterator!=faces.end();i++) iterator = faces.erase(iterator);
//        Face nf(f1.v1,f2.v3,f3.v2, f.level-1);
        //        newFaces.push_back(nf);
        delete iterator->child0;
        delete iterator->child1;
        delete iterator->child2;
        delete iterator->child3;
        
        return true;
    }
    else { ++iterator; return false; }
}

void Planet::Update(Player& player)
{
    bool wasSubdivided = false;
    std::vector<Face> newFaces;
    for (auto it = faces.begin();it!=faces.end();it++)
    {
        
//        if (tryCombine(it, [this](Player& player, Face f)->bool { return std::min(std::min(
//                                                                                                glm::length(-player.Camera.Position - f.v1),
//                                                                                                glm::length(-player.Camera.Position - f.v2)),
//                                                                                       glm::length(-player.Camera.Position - f.v3)) >= (float)(1 << (LOD_MULTIPLIER)) / ((float)(1 << (f.level-1))); }, player, newFaces)) wasSubdivided=true;
           if (recursiveSubdivide(&(*it), player))
              wasSubdivided=true;
        
    }
//    for (Face f : newFaces) faces.push_back(f);
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
        vertices.push_back(Vertex(glm::vec4(face.v1,1.0f)));
        vertices.push_back(Vertex(glm::vec4(face.v2,1.0f)));
        vertices.push_back(Vertex(glm::vec4(face.v3,1.0f)));
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
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
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