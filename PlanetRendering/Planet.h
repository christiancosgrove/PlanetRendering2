#pragma once

//
//  Planet.h
//  PlanetRendering
//
//  Created by Christian on 9/28/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#include <OpenGL/gl3.h>
#include <functional>
#include "Player.h"
#include <vector>
#include "GLManager.h"

struct Face
{
    glm::vec3 v1,v2,v3;
    unsigned int level;
    Face() : level(0) {}
    Face(glm::vec3 _v1, glm::vec3 _v2, glm::vec3 _v3) : v1(_v1), v2(_v2), v3(_v3), level(0)
    {
        
    }
    Face(glm::vec3 _v1, glm::vec3 _v2, glm::vec3 _v3, unsigned int _level) : v1(_v1), v2(_v2), v3(_v3), level(_level)
    {
        
    }
    
    Face(const Face& face) : v1(face.v1), v2(face.v2), v3(face.v3), level(face.level) {}
    //Face& operator=(Face& rhs) { v1=rhs.v1;v2=rhs.v2;v3=rhs.v3;return *this; }
    
    //Face& operator=(Face& rhs) {memcpy(this, &rhs, sizeof(Face)); return *this; }
};

struct Vertex
{
    float x,y,z,w;
    Vertex(glm::vec4 pos) : x(pos.x), y(pos.y), z(pos.z), w(pos.w) {}
};

//TODO: implement vertex indexing

//reference on subdivided icosahedrons:
//http://www.donhavey.com/blog/tutorials/tutorial-3-the-icosahedron-sphere/
class Planet
{
public:
    
    glm::vec3 Position;
    float Radius;
    const int LOD_MULTIPLIER=4;
    
    Planet(glm::vec3 pos, float radius);
    ~Planet();
    
    void Update(Player& player);
    
    void Draw(Player& player, GLManager& glManager);
    
    
    
    
    inline float terrainNoise(float x, float y, float z);
    inline float terrainNoise(glm::vec3 v);
private:
    std::vector<Face> faces;
    std::vector<Vertex> vertices;
    
    GLuint VBO;
    GLuint VAO;
    
    //takes a function of the player information and the current face
    bool trySubdivide(std::vector<Face>::iterator& iterator, const std::function<bool(Player&, Face)>& func, Player& player, std::vector<Face>& newFaces);
    bool tryCombine(std::vector<Face>::iterator& iterator, const std::function<bool(Player&, Face)>& func, Player& player, std::vector<Face>& newFaces);
    inline void projectFaceOntoSphere(Face& f);
    void buildBaseMesh();
    void generateBuffers();
    void updateVBO();
};

inline float randFloat()
{
    return (float)rand() / RAND_MAX;
}

float Planet::terrainNoise(float x, float y, float z)
{
    return 0;//0.9 * sin(sin(((0.1*x + 0.1*y - 0.001*z))*0.1)) + 0.005*sin(sin((x + y + z)*100));
}

float Planet::terrainNoise(glm::vec3 v)
{
    return terrainNoise(v.x,v.y,v.z);
}

void Planet::projectFaceOntoSphere(Face &f)
{
    
}


