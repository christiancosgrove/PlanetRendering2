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
    glm::dvec3 v1,v2,v3;
    Face* child0;
    Face* child1;
    Face* child2;
    Face* child3;
    unsigned int level;
    Face() : level(0) {}
    Face(glm::dvec3 _v1, glm::dvec3 _v2, glm::dvec3 _v3) : v1(_v1), v2(_v2), v3(_v3), level(0), child0(nullptr),child1(nullptr), child2(nullptr), child3(nullptr)
    {
        
    }
    Face(glm::dvec3 _v1, glm::dvec3 _v2, glm::dvec3 _v3, unsigned int _level) : v1(_v1), v2(_v2), v3(_v3), level(_level),child0(nullptr), child1(nullptr), child2(nullptr), child3(nullptr)
    {
        
    }
    
    Face(const Face& face) : v1(face.v1), v2(face.v2), v3(face.v3), level(face.level), child0(face.child0), child1(face.child1), child2(face.child2),child3(face.child3)  {}
    //Face& operator=(Face& rhs) { v1=rhs.v1;v2=rhs.v2;v3=rhs.v3;return *this; }
    
    //Face& operator=(Face& rhs) {memcpy(this, &rhs, sizeof(Face)); return *this; }
};

struct Vertex
{
    double x,y,z,w;
    Vertex(glm::dvec4 pos) : x(pos.x), y(pos.y), z(pos.z), w(pos.w) {}
};

//TODO: implement vertex indexing

//reference on subdivided icosahedrons:
//http://www.donhavey.com/blog/tutorials/tutorial-3-the-icosahedron-sphere/
class Planet
{
public:
    
    glm::vec3 Position;
    double Radius;
    const int LOD_MULTIPLIER=5;
    
    Planet(glm::vec3 pos, float radius);
    ~Planet();
    
    void Update(Player& player);
    
    void Draw(Player& player, GLManager& glManager);
    
    
    
    
    inline double terrainNoise(double x, double y, double z);
    inline double terrainNoise(glm::dvec3 v);
private:
    std::vector<Face> faces;
    std::vector<Vertex> vertices;
    
    GLuint VBO;
    GLuint VAO;
    //TODO: implement vertex indexing for faster rendering and less CPU-GPU communcation
    
    //takes a function of the player information and the current face
    bool trySubdivide(Face* face, const std::function<bool(Player&, const Face&)>& func, Player& player);
    bool tryCombine(Face* face, const std::function<bool(Player&, const Face&)>& func, Player& player);
    inline void projectFaceOntoSphere(Face& f);
    inline double randDouble(double seedx, double seedy);
    inline double randDouble(glm::dvec2 vec);
    inline glm::dvec2 sphericalCoordinates(glm::dvec3 pos);
    void buildBaseMesh();
    void generateBuffers();
    void updateVBO();
    void recursiveUpdate(Face& face);
    bool recursiveSubdivide(Face* face, Player& player);
    bool recursiveCombine(Face* face, Player& player);
    void combineFace(Face* face);
    int time;
};

double Planet::randDouble(double seedx, double seedy)
{
    double fract;
    return std::modf(sin((12.9898 * seedx + 78.233 * seedy)*43758.5453), &fract);
    
}
double Planet::randDouble(glm::dvec2 vec)
{
    return randDouble(vec.x, vec.y);
}

glm::dvec2 Planet::sphericalCoordinates(glm::dvec3 pos)
{
    return glm::dvec2(std::atan2(pos.y, pos.x), std::atan2(pos.z, sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z)));
}

double Planet::terrainNoise(double x, double y, double z)
{
    return 0.01*randDouble(sphericalCoordinates(glm::dvec3(x,y,z)));//0.9 * sin(sin(((0.1*x + 0.1*y - 0.001*z))*0.1)) + 0.005*sin(sin((x + y + z)*100));
}

double Planet::terrainNoise(glm::dvec3 v)
{
    return terrainNoise(v.x,v.y,v.z);
}

void Planet::projectFaceOntoSphere(Face &f)
{
    
}


