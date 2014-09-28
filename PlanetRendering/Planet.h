//
//  Planet.h
//  PlanetRendering
//
//  Created by Christian on 9/28/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#ifndef __PlanetRendering__Planet__
#define __PlanetRendering__Planet__

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <functional>
#include "Player.h"

struct Face
{
    sf::Vector3f v1,v2,v3;
    Face(sf::Vector3f _v1, sf::Vector3f _v2, sf::Vector3f _v3) : v1(_v1), v2(_v2), v3(_v3)
    {
        
    }
    //Face(const Face& face) : v1(face.v1), v2(face.v2), v3(face.v3) {}
    //Face& operator=(Face& rhs) { v1=rhs.v1;v2=rhs.v2;v3=rhs.v3;return *this; }
    
    //Face& operator=(Face& rhs) {memcpy(this, &rhs, sizeof(Face)); return *this; }
};

//TODO: implement vertex indexing

//reference on subdivided icosahedrons:
//http://www.donhavey.com/blog/tutorials/tutorial-3-the-icosahedron-sphere/
class Planet
{
public:
    
    sf::Vector3f Position;
    float Radius;
    
    
    Planet(sf::Vector3f pos, float radius);
    
    void Update(Player& player);
    
    void Draw();
    
private:
    std::vector<Face> faces;
    
    GLuint VBO;
    
    //takes a function of the player information and the current face
    bool trySubdivide(std::vector<Face>::iterator& iterator, const std::function<bool(Player&, std::vector<Face>::iterator)>& func, Player& player);
    inline void projectFaceOntoSphere(Face& f);
    void buildBaseMesh();
    void generateBuffers();
    void updateVBO();
};

void Planet::projectFaceOntoSphere(Face &f)
{
    
}


#endif /* defined(__PlanetRendering__Planet__) */
