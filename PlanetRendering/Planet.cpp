//
//  Planet.cpp
//  PlanetRendering
//
//  Created by Christian on 9/28/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#include "Planet.h"
#include "VectorHelper.h"

Planet::Planet(sf::Vector3f pos, float radius) : Position(pos), Radius(radius)
{
    buildBaseMesh();
    generateBuffers();
}

bool Planet::trySubdivide(std::vector<Face>::iterator& iterator, const std::function<bool (Player&, std::vector<Face>::iterator)>& func, Player& player)
{
    
    Face f = *iterator;
    
    if (func(player, iterator))
    {
        sf::Vector3f m12 = Normalize((f.v1 + f.v2) * 0.5f)*Radius;
        sf::Vector3f m13 = Normalize((f.v1 + f.v3) * 0.5f)*Radius;
        sf::Vector3f m23 = Normalize((f.v3 + f.v3) * 0.5f)*Radius;
        
        Face f1(m13,m23,m12);
        Face f2(f.v1,m13,m12);
        Face f3(m12,m23,f.v2);
        Face f4(m13,f.v3,m23);
        
        iterator=faces.erase(iterator);
        faces.push_back(f1);
        faces.push_back(f2);
        faces.push_back(f3);
        faces.push_back(f4);
        return true;
    }
    else { ++iterator; return false; }
}

void Planet::Update(Player& player)
{
    bool wasSubdivided = false;
    for (auto it = faces.begin();it!=faces.end();)
    {
        if (trySubdivide(it, [](Player& player, std::vector<Face>::iterator it)->bool { return false; }, player)) wasSubdivided=true;
    }
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
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Face) * faces.size(), &faces[0], GL_STATIC_DRAW);
}


void Planet::buildBaseMesh()
{
    faces.push_back(Face(sf::Vector3f(0,0,1),sf::Vector3f(1,0,0),sf::Vector3f(0,1,0)));
}

void Planet::Draw()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableVertexAttribArray(0);
    
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexPointer(3, GL_FLOAT, 0, &faces[0]);
    glDrawArrays(GL_TRIANGLES, 0, faces.size()*3);
    
    //glDisableVertexAttribArray(0);
    glDisableClientState(GL_VERTEX_ARRAY);
}