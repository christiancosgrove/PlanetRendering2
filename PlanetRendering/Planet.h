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
#include "typedefs.h"
#include <thread>
#include "glm/gtx/norm.hpp"
#include "PlanetAtmosphere.h"

//Representation of a triangular face on CPU side of program
//represents a single node in the face tree
struct Face
{
    //vertices of triangle
    vvec3 v1,v2,v3;
    //pointers to children in tree structure
    Face* child0;
    Face* child1;
    Face* child2;
    Face* child3;
    //depth in tree
    unsigned int level;
    Face() : level(0) {}
    ~Face()
    {
    }
    
    Face(vvec3 _v1, vvec3 _v2, vvec3 _v3) : v1(_v1), v2(_v2), v3(_v3), level(0), child0(nullptr),child1(nullptr), child2(nullptr), child3(nullptr)
    {
        
    }
    Face(vvec3 _v1, vvec3 _v2, vvec3 _v3, unsigned int _level) : v1(_v1), v2(_v2), v3(_v3), level(_level),child0(nullptr), child1(nullptr), child2(nullptr), child3(nullptr)
    {
        
    }
    //copy constructor
    Face(const Face& face) : v1(face.v1), v2(face.v2), v3(face.v3), level(face.level), child0(face.child0), child1(face.child1), child2(face.child2),child3(face.child3)  {}
    //returns the normal vector of the face (used for lighting calculations)
    inline vvec3 GetNormal()
    {
        return glm::normalize(glm::cross(v1 - v2, v1 - v3));
    }
    
};
//Representation of a single vertex for communication with GPU
//recursiveUpdate function interops between two representations of vertex data (convenient Face & rendering Vertex representations)
//Contains position as well as normal vectors
//Due to the number of vertices, would like to implement vertex indexing (essentially ignoring duplicated vertices)
struct Vertex
{
    vfloat x,y,z;
    vfloat nx, ny, nz;
    Vertex(vvec3 pos, vvec3 normal) : x(pos.x), y(pos.y), z(pos.z),
    nx(normal.x), ny(normal.y), nz(normal.z)
    {}
};

//TODO: implement vertex indexing

//reference on subdivided icosahedrons (the geodesic sphere):
//http://www.donhavey.com/blog/tutorials/tutorial-3-the-icosahedron-sphere/


//This class contains functionality for rendering and generating the procedural planet
class Planet
{
public:
    enum class RenderMode
    {
        SOLID,
        WIRE,
    };
    RenderMode CurrentRenderMode;
    //Position is defaulted to origin (shaders may not work if pos!=origin right now)
    vvec3 Position;
    //radius of planet in device coordinates (default 1)
    vfloat Radius;
    const vfloat EARTH_DIAMETER = 12756200.0;
    //number in [0,1] reflecting how fractal-like the terrain is (larger values lead to more variation at smaller scales)
    const float TERRAIN_REGULARITY = 0.33;
    
    //Number of levels of detail (impacts rendering performance)
    //Multiplies average # of vertices by 4^N
    const int LOD_MULTIPLIER=6;
    
    float SeaLevel;
    
    enum class RotationMode
    {
        NO_ROTATION,
        ROTATION
    };
    
    RotationMode CurrentRotationMode; // determines whether the planet rotates relative to the player or relative to the star
    //Radians/tick rotation rate of sun around planet
    float ROTATION_RATE;
    
    //Seed used for random number generator (RNG needs to be updates)
    const vfloat SEED;
    //Initialization of planet
    Planet(glm::vec3 pos, vfloat radius, vfloat seed,Player& _player, GLManager& _glManager);
    //De-initialization of planet (destruction of GL objects)
    ~Planet();
    //Perform subdivisions/combinations accordingly, update vertex buffers
    void Update();
    //Render planet with Vertex Buffer Object/Vertex Array Object
    void Draw(Player& player, GLManager& glManager);
    //Terrain generation function in cartesion coordinates (spherically-symmetric)
    inline vfloat terrainNoise(vfloat x, vfloat y, vfloat z);
    inline vfloat terrainNoise(vvec3 v);
private:
    //Planet faces.  This array only contains the base icosahedron vertices, and deeper faces are stored on the heap (in a tree structure).  These are not directly transferred to the GPU
    std::vector<Face> faces;
    //Array of vertices.  This array is generated every time the geometry is updated (perhaps this can be optimized) and is copied directly to the GPU.
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    //VBO=Vertex Buffer Object.  This OpenGL API object contains functionality for sending arrays of vertices (with arbitrary attributes) to the GPU.  The attributes of each vertex can be referenced in the vertex shader.
    GLuint VBO;
    //VAO=Vertex Array Object.  This OpenGL API object contains functionality for saving the configuration of vertex arrays (i.e. pointers to attributes).
    GLuint VAO;
    GLuint IBO;
    
    GLManager& glManager;
    Player& player;
    std::mutex renderMutex;
    
    PlanetAtmosphere atmosphere;
    
    inline bool inHorizon(vvec3 vertex);
    
    //TODO: implement vertex indexing for faster rendering and less CPU-GPU communcation
    
    //This function, which accepts a face and a boolean-valued function of the player's position and that face, checks whether a face is ready to be subdivided (in this case, close enough to the camera) and performs the subdivision.  The function argument of this method makes it more modular; the function used to CHECK whether to subdivide the face is external.
    //takes a function of the player information and the current face
    bool trySubdivide(Face* face, const std::function<bool(Player&, const Face&)>& func, Player& player);
    //Like trySubdivide, this function accepts a face and a function of the face and the player.  Instead of subdividing the face if it meets the function's criteria, it instead COMBINES the face's children.  Also like trySubdivide, this function is heavily reliant on the tree structure of the faces (one may now see why it was chosen over a one-dimensional resizeable array).
    bool tryCombine(Face* face, const std::function<bool(Player&, const Face&)>& func, Player& player);
    //This function is a pseudorandom number generator of two arguments (in this case the polar and azimuthal angles of the vertex in spherical coordinates)
    inline vfloat randvfloat(vfloat seedx, vfloat seedy);
    inline vfloat randvfloat(vvec2 vec);
    //converts a Cartesian vector to a two-dimesnional polar-azimuthal vector (ignores radius)
    inline glm::dvec2 sphericalCoordinates(vvec3 pos);
    //construct base icosahedron
    void buildBaseMesh();
    //initialize VBO and VAO
    void generateBuffers();
    //Reconstruct vertex array and send vertices to GPU
    void updateVBO(Player& player);
    //Append vertices deepest in the tree to vertex array to be sent to GPU
    //also calculates the player's minimum distance to the planet surface
    void recursiveUpdate(Face& face, unsigned int index1, unsigned int index2, unsigned int index3, Player& player, std::vector<Vertex>& newVertices, std::vector<unsigned int>& newIndices);
    //Perform trySubdivide by recursively traversing tree
    bool recursiveSubdivide(Face* face, Player& player);
    //Perform tryCombine by recursively traversing tree
    bool recursiveCombine(Face* face, Player& player);
    //Simple function which deletes children vertices in order to combine the face.
    void combineFace(Face* face);
    void setUniforms();
    //number of ticks (executions of Update()) since start; used in rotation of sun
    float time;
    bool closed;
    bool subdivided;
    unsigned int prevVerticesSize;
};


bool Planet::inHorizon(vvec3 vertex)
{
    vfloat playerHeight = glm::length(player.Camera.GetPosition())-1.0;
    //refer to Wikipedia for formula for horizon distance
    vfloat horizonDist2 = 2*Radius * playerHeight + playerHeight * playerHeight;
    vfloat dist2 = glm::length2(vertex + player.Camera.GetPosition());
    return (std::max(horizonDist2,static_cast<vfloat>(0.2)) > dist2);
}

//TODO: need new, more efficent RNG
vfloat Planet::randvfloat(vfloat seedx, vfloat seedy)
{
    vfloat fract;
    return std::modf(sin((12.9898 * (seedx+SEED) + 78.233 * (seedy+SEED))*437586142312314.5453), &fract);
    
    
}
vfloat Planet::randvfloat(vvec2 vec)
{
    return randvfloat(vec.x, vec.y);
}

glm::dvec2 Planet::sphericalCoordinates(vvec3 pos)
{
    return glm::dvec2(std::atan2(pos.y, pos.x), std::atan2(pos.z, sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z)));
}

vfloat Planet::terrainNoise(vfloat x, vfloat y, vfloat z)
{
    return 0.01*randvfloat(sphericalCoordinates(vvec3(x,y,z)));//0.9 * sin(sin(((0.1*x + 0.1*y - 0.001*z))*0.1)) + 0.005*sin(sin((x + y + z)*100));
}

vfloat Planet::terrainNoise(vvec3 v)
{
    return terrainNoise(v.x,v.y,v.z);
}

