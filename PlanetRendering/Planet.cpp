//
//  Planet.cpp
//  PlanetRendering
//
//  Created by Christian on 9/28/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#include "Planet.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <algorithm>
#include <random>
#include <time.h>
#include "MainGame_SDL.h"
#include <thread>
#include "RandomUtils.h"
#include<fstream>
#include "ResourcePath.hpp"
#include "AABB.h"

//Constructor for planet.  Initializes VBO (experimental) and builds the base icosahedron mesh.
Planet::Planet(int planetIndex, glm::vec3 pos, vfloat radius, double mass, vfloat seed, Player& _player, GLManager& _glManager, float terrainRegularity)
:
performanceOutput("planet" + std::to_string(planetIndex) + ".csv"),
Radius(radius),
time(0),
SEED(seed),
CurrentRenderMode(RenderMode::SOLID),
player(_player),
glManager(_glManager),
closed(false),
CurrentRotationMode(RotationMode::NO_ROTATION),
atmosphere(pos, radius*1.01),
PlanetInfo{
//    glm::vec4(0.0945,0.2011,0.45,1.),
//    glm::vec4(0.0867,0.3336,0.51,1.),
//    glm::vec4(0.0513,0.0768,0.27,1.),
//    glm::vec4(0.1438, 0.22, 0.0814,1.),
    //    glm::vec4(0.8, 0.760784, 0.470588,1.),
    glm::vec4(RandomUtils::Uniform<float>(0,0.9f),RandomUtils::Uniform<float>(0,0.9f),RandomUtils::Uniform<float>(0,0.9f),1.),
    glm::vec4(RandomUtils::Uniform<float>(0,0.9f),RandomUtils::Uniform<float>(0,0.9f),RandomUtils::Uniform<float>(0,0.9f),1.),
    glm::vec4(RandomUtils::Uniform<float>(0,0.9f),RandomUtils::Uniform<float>(0,0.9f),RandomUtils::Uniform<float>(0,0.9f),1.),
    glm::vec4(RandomUtils::Uniform<float>(0,0.9f),RandomUtils::Uniform<float>(0,0.9f),RandomUtils::Uniform<float>(0,0.9f),1.),
    glm::vec4(RandomUtils::Uniform<float>(0,0.9f),RandomUtils::Uniform<float>(0,0.9f),RandomUtils::Uniform<float>(0,0.9f),1.),
    glm::vec4(RandomUtils::Uniform<float>(0,0.9f),RandomUtils::Uniform<float>(0,0.9f),RandomUtils::Uniform<float>(0,0.9f),1.),
    vmat4(),
    0.001f,
    0.005f,
    1.0f},
PhysicsObject(static_cast<glm::dvec3>(pos), mass),
TERRAIN_REGULARITY(terrainRegularity),
Angle(0),
AngularVelocity(100.,0.0,0)
//5.972E24)
{
    lastPlayerUpdatePosition=player.Position;
    std::ofstream stream(resourcePath() + performanceOutput, std::ios::out);
    generateBuffers();
    buildBaseMesh();
    //create a separate thread in which updates occur
    std::thread t(&Planet::Update, this);
    updateThread.swap(t);
    
}

//clear allocated memory and OpenGL objects
Planet::~Planet()
{
    closed = true;
    glDeleteVertexArrays(1, &VAO);
    for (auto it = faces.begin(); it!=faces.end();)
    {
        it = faces.erase(it);
    }
    updateThread.join();
}

inline vfloat pointLineDist(vvec2 point1, vvec2 point2, vvec2 point);
bool Planet::faceInView(const Face& f)
{
    
    std::array<vvec2,3> transformedVertices;
    for (int i = 0; i<3;i++)
    {
        vvec4 v = PlanetInfo.transformMatrix * vvec4(f.vertices[i],1.0);
        transformedVertices[i]=vvec2(v.x/v.w,v.y/v.w);
    }
    
    return AABB::Intersection(AABB(0,0,2,2),AABB(transformedVertices));
//    for (vvec2 vert:v)
//    {
//        if (pointLineDist(transformedVertices[0], transformedVertices[1],vert)<0 &&
//            pointLineDist(transformedVertices[1], transformedVertices[2],vert)<0 &&
//            pointLineDist(transformedVertices[2], transformedVertices[0],vert)<0)
//            return true;
//    }
//    return false;
}
//signed distance
vfloat pointLineDist(vvec2 point1, vvec2 point2, vvec2 point)
{
    return (glm::dot(vvec2(point2.y-point1.y,point1.x-point2.x),point)+point2.x*point1.y-point2.y*point1.x)/glm::length(point2-point1);
}


//This function accepts a boolean-valued function of displacement.  If the function is true, the function will divide the given face into four subfaces, each of which will be recursively subjected to the same subdivision scheme.  It is important that the input function terminates at a particular level of detail, or the program will crash.
bool Planet::trySubdivide(Face* iterator, const std::function<bool (Player&, const Face&)>& func, Player& player)
{
    //perform horizon culling
//    if (iterator->level!=0 && !inHorizon(*iterator))  return false;
    if (iterator->level>MAX_LOD) return false;
    
    if (closed) return false;
    
    if (func(player, *iterator))
    {
        if (!iterator->AllChildrenNull())
            return true;
        
        //face vertices
        std::array<vvec3, 3> v;
        for (int i = 0; i<3;i++)
            v[i]=iterator->vertices[i];
        
        std::array<vvec2,3> p;
        for (int i = 0; i<3;i++)
            p[i]=iterator->polarCoords[i];
        
        std::array<vvec3, 3> nv;
        for (int i = 0;i<3;i++)
            nv[i]=glm::normalize(v[i]);
        
        //lengths of face vertices
        std::array<vfloat,3> l;
        for (int i = 0; i<3;i++)
            l[i]=glm::length(v[i])/Radius;
        
        
        //normalized midpoints of face vertices
        vvec3 m12 = glm::normalize((nv[0] + nv[1]) * static_cast<vfloat>(0.5));
        vvec3 m13 = glm::normalize((nv[0] + nv[2]) * static_cast<vfloat>(0.5));
        vvec3 m23 = glm::normalize((nv[1] + nv[2]) * static_cast<vfloat>(0.5));
        //normalized midpoints of face vertices (polar coords)
//        vvec2 p12((v1.x+v2.x) / 2,(v1.y+v2.y) / 2);
//        vvec2 p13((v1.x+v3.x) / 2,(v1.y+v3.y) / 2);
//        vvec2 p23((v2.x+v3.x) / 2,(v2.y+v3.y) / 2);
        glm::dvec2 p12((v[0].x+v[1].x) / 2,(v[0].y+v[1].y) / 2);
        glm::dvec2 p13((v[0].x+v[2].x) / 2,(v[0].y+v[2].y) / 2);
        glm::dvec2 p23((v[1].x+v[2].x) / 2,(v[1].y+v[2].y) / 2);
        //height scale of terrain
        //proportional to 2^(-LOD) * nonlinear factor
        //the nonlinear factor is LOD^(TERRAIN_REGULARITY)
        //if the nonlinear factor is 1, the terrain is boring -- this is introduced to make higher-frequency noise more noticeable.
        vfloat fac =static_cast<vfloat>(3.)/static_cast<vfloat>(1 << iterator->level)*std::pow(static_cast<vfloat>(iterator->level+1), TERRAIN_REGULARITY);
        
        m12*=1 + terrainNoise(p12) * fac;
        m13*=1 + terrainNoise(p13) * fac;
        m23*=1 + terrainNoise(p23) * fac;
        
        m12*=(l[0] + l[1])/static_cast<vfloat>(2.)*Radius;
        m13*=(l[0] + l[2])/static_cast<vfloat>(2.)*Radius;
        m23*=(l[1] + l[2])/static_cast<vfloat>(2.)*Radius;
        
//        m12+=Position;
//        m13+=Position;
//        m23+=Position;
        Face *f0,*f1,*f2,*f3;
        
        {
            std::lock_guard<std::mutex> lock(renderMutex);
            f0 = new Face(iterator,m13,m12,m23,p13,p12,p23,iterator->level+1);
            f1 = new Face(iterator,v[2],m13,m23,p[2],p13,p23,iterator->level+1);
            f2 = new Face(iterator,m23,m12,v[1],p23,p12,p[1],iterator->level+1);
            f3 = new Face(iterator,m13,v[0],m12,p13,p[0],p12,iterator->level+1);
        }
        iterator->children = {f0, f1, f2, f3};
        
        return true;
    }
    return false;
}
//Similarly to trySubdivide, this function combines four faces into a larger face if a boolean-valued function is statisfied.
bool Planet::tryCombine(Face* iterator, const std::function<bool (Player&, const Face&)>& func, Player& player)
{
    if (closed) return false;
    if (iterator==nullptr) return false;
    if (iterator->AnyChildrenNull()) return false;
    if ((func(player, *iterator) || func(player, *iterator->children[0])|| func(player, *iterator->children[1]) || func(player, *iterator->children[2]) || func(player, *iterator->children[3])) && iterator->level>0)
    {
        combineFace(iterator);
        
        return true;
    }
    return false;
}

void Planet::combineFace(Face* face)
{
    if (closed) return;
    if (face->level==0) return;
    for (Face*& f : face->children)
        if (f!=nullptr)
        {
            combineFace(f);
            delete f;
            f = nullptr;
        }
}
//performed in background, manages terrain generation
void Planet::Update()
{
    
    if (closed) return;
    subdivided = false;
    //iterate through faces and perform necessary generation checks
    
    
    auto t = std::chrono::high_resolution_clock::now();
    
    for (auto it = faces.begin();it!=faces.end();it++)
    {
        if (recursiveCombine(&(*it), player))
            subdivided=true;
        if (recursiveSubdivide(&(*it), player))
            subdivided=true;
    }
//    printf("1 time taken: %lli us\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t).count());
    
    //update vertices if changes were made
    
    t = std::chrono::high_resolution_clock::now();
    
    unsigned int indsize, vertsize;
    GetIndicesVerticesSizes(indsize, vertsize);
    if (subdivided || vertsize==0)
    {
        updateVBO(player);
        lastPlayerUpdatePosition=player.Position;
    }
//    printf("2 time taken: %lli us\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t).count());
//    std::cout << "Height above earth surface: " << player.DistFromSurface * EARTH_DIAMETER << " m\n";
    //repeat indefinitely (on separate thread)
    
    Update();
}

void Planet::generateBuffers()
{
    //generate vertex array object -- contains state data for other relevant OpenGL objects
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    //set up vertex attributes.  These contain position and normal data for each vertex.
    //Use preprocessor conditionals to differentiate between two possible precisions
#ifdef VERTEX_DOUBLE
    glVertexAttribLPointer(0, 3, GL_DOUBLE, sizeof(Vertex), (void*)__offsetof(Vertex, x));
    glVertexAttribLPointer(1, 2, GL_DOUBLE, sizeof(Vertex), (void*)__offsetof(Vertex, tx));
    glVertexAttribLPointer(2, 3, GL_DOUBLE, sizeof(Vertex), (void*)__offsetof(Vertex, nx));
#else
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),(void*)__offsetof(Vertex, x));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),(void*)__offsetof(Vertex, tx));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),(void*)__offsetof(Vertex, nx));
#endif
    //unbind VAO
    glBindVertexArray(0);
}


//Deprecated.  This function is a higher-performance alternative to the currently implemented sorting scheme in updateVBO().
//It is faster (sometimes by a factor of 3), but it produces non-ideal normal discontinuities (mainly due to the recursive implementation of the function).
void Planet::recursiveUpdate(Face& face, unsigned int index1, unsigned int index2, unsigned int index3, Player& player, std::vector<Vertex>& newVertices, std::vector<unsigned int>& newIndices)
{
    if (closed) return;
    //Get player distance from face.  If temporary minimum, set player distance (this process naturally finds the player's minimum distance to the surface).
    {
        std::lock_guard<std::mutex> lock(renderMutex);
        vfloat dist = std::min(std::min(glm::length(GetPlayerDisplacement() - face.vertices[0]),glm::length(GetPlayerDisplacement() - face.vertices[1])),glm::length(GetPlayerDisplacement()- face.vertices[2]));
        if (player.DistFromSurface > dist) player.DistFromSurface = dist;
    }
    //perform horizon culling
    if (face.level!=0 && !inHorizon(face)) return;
    if (!faceInView(face)) return;
    if (!face.AnyChildrenNull())
    {
        vvec3 norm = face.GetNormal();
        unsigned int ni1,ni2,ni3; //new indices
        unsigned int currIndex=newVertices.size();
        if (face.level==0)
        {
            newVertices.push_back(Vertex(face.vertices[0], (vvec2)face.polarCoords[0], norm));
            newVertices.push_back(Vertex(face.vertices[1], (vvec2)face.polarCoords[1], norm));
            newVertices.push_back(Vertex(face.vertices[2], (vvec2)face.polarCoords[2], norm));
            index1 = currIndex + 0;
            index2 = currIndex + 1;
            index3 = currIndex + 2;
        }
        currIndex = newVertices.size();
        
        vvec3 norm0 = face.children[0]->GetNormal();
        vvec3 norm1 = face.children[1]->GetNormal();
        vvec3 norm2 = face.children[2]->GetNormal();
        vvec3 norm3 = face.children[3]->GetNormal();
        
        
        newVertices.push_back(Vertex(face.children[0]->vertices[0], (vvec2)face.children[0]->polarCoords[0], glm::normalize(norm0 + norm1 + norm3)));
        newVertices.push_back(Vertex(face.children[0]->vertices[1], (vvec2)face.children[0]->polarCoords[1], glm::normalize(norm0 + norm1 + norm2)));
        newVertices.push_back(Vertex(face.children[0]->vertices[2], (vvec2)face.children[0]->polarCoords[2], glm::normalize(norm0 + norm2 + norm3)));
        ni1 = currIndex + 0;
        ni2 = currIndex + 1;
        ni3 = currIndex + 2;
        
        recursiveUpdate(*face.children[0], ni1, ni2, ni3, player, newVertices, newIndices);
        recursiveUpdate(*face.children[1], index3, ni1, ni3, player,newVertices, newIndices);
        recursiveUpdate(*face.children[2], ni3, ni2, index2, player,newVertices, newIndices);
        recursiveUpdate(*face.children[3], ni1, index1,ni2, player,newVertices, newIndices);
    }
    else
    {
        newIndices.push_back(index1);
        newIndices.push_back(index2);
        newIndices.push_back(index3);
    }
}


bool Planet::recursiveSubdivide(Face* face, Player& player)
{
    if (trySubdivide(face,
                     [this](Player& player, Face f)->bool {
                         return std::max(std::max(
                                                  glm::length(GetPlayerDisplacement() - f.vertices[0]),
                                                  glm::length(GetPlayerDisplacement() - f.vertices[1])),
                                         glm::length(GetPlayerDisplacement() - f.vertices[2]))
                         < (float)(1 << LOD_MULTIPLIER) / ((float)(1 << (f.level))); }
                     , player))
    {
        for (Face* f:face->children) recursiveSubdivide(f, player);
        return true;
    }
    return false;
}


bool Planet::recursiveCombine(Face* face, Player& player)
{
    if (closed) return false;
    if (face==nullptr) return false;
    if (!tryCombine(face,
                    
                    [this](Player& player, Face f)->bool { return std::min(std::min(
                                                                                    glm::length(GetPlayerDisplacement() - f.vertices[0]),
                                                                                    glm::length(GetPlayerDisplacement() - f.vertices[1])),
                                                                           glm::length(GetPlayerDisplacement() - f.vertices[2] - static_cast<vvec3>(Position)))
                        >= (double)(1 << (LOD_MULTIPLIER)) / ((double)(1 << (f.level-1))); }
                    
                    , player))
    {
        if (face->parent!=nullptr)
        {
            recursiveCombine(face->parent, player);
        }
        return false;
    }
    else return true;
}

void Planet::getRootFaces(std::vector<Face*>& rootFaces, Player& player)
{
    for (Face& f:faces)
        recursiveGetRootFaces(rootFaces, &f,player);
}
void Planet::recursiveGetRootFaces(std::vector<Face *> &rootFaces, Face* f, Player& player)
{
    if (f==nullptr) return;
    if (!inHorizon(*f) && f->level!=0) return;
//    if (!faceInView(*f)) return;
    for (auto& i:f->indices)i=-1;
    if (f->AllChildrenNull())
        rootFaces.push_back(f);
    else
        for (Face* child:f->children)
            recursiveGetRootFaces(rootFaces, child,player);
}

void Planet::updateVBO(Player& player)
{
    const vfloat displacementThreshold=0.1;
    auto t = std::chrono::high_resolution_clock::now();
    unsigned int indsize, vertsize;
    GetIndicesVerticesSizes(indsize, vertsize);
    std::vector<Vertex> newVertices;
    newVertices.reserve(vertsize);
    std::vector<unsigned int> newIndices;
    newIndices.reserve(indsize);
    player.DistFromSurface=10.;
    
#ifdef SMOOTH_FACES
    
    std::vector<Face*> rootFaces;
    getRootFaces(rootFaces,player);
    std::vector<std::pair<int, int>> verticesSorted(rootFaces.size()*3);
    
    for (int i = 0; i<rootFaces.size();i++)
    {
        verticesSorted[3*i] = std::pair<int, int>(0,i);
        verticesSorted[3*i+1] = std::pair<int, int>(1,i);
        verticesSorted[3*i+2] = std::pair<int, int>(2,i);
    }
    
    
    const auto func =[this, &rootFaces](std::pair<int, int>& p1, std::pair<int, int>& p2)
    {
        glm::dvec2 c1=rootFaces[p1.second]->polarCoords[p1.first] + glm::dvec2(M_PI,M_2_PI);
        glm::dvec2 c2=rootFaces[p2.second]->polarCoords[p2.first] + glm::dvec2(M_PI,M_2_PI);
        
        //                  return c1.x + 2 * Radius * c1.y + 4 * Radius * Radius * c1.z <
        //                  c2.x + 2 * Radius * c2.y + 4 * Radius * Radius * c2.z;
        return c1.x + 2*M_2_PI * c1.y < c2.x + 2*M_2_PI*c2.y;
    };
    //    if (getPlayerDisplacementSquared(player)>displacementThreshold) return;
    t = std::chrono::high_resolution_clock::now();

#ifdef USE_HEAP
    std::make_heap(verticesSorted.begin(), verticesSorted.end(),func);
    std::sort_heap(verticesSorted.begin(),verticesSorted.end(),func);
#else
    std::sort(verticesSorted.begin(), verticesSorted.end(),func);
#endif
    //    if (getPlayerDisplacementSquared(player)>displacementThreshold) return;
    printf("Time: %lli\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t).count());
        for (int j = 0; j<verticesSorted.size();j++)
    {
        const int maxNeighbors=6;
        
        if (rootFaces.at(verticesSorted[j].second)->indices.at(verticesSorted[j].first)!=-1) continue;
        int currentSize = newVertices.size();
        
        vvec3 normal = rootFaces.at(verticesSorted[j].second)->GetNormal();
        
        newVertices.push_back(Vertex(rootFaces.at(verticesSorted[j].second)->vertices.at(verticesSorted[j].first),(vvec2)rootFaces.at(verticesSorted[j].second)->polarCoords.at(verticesSorted[j].first), glm::vec3()));
        
        rootFaces.at(verticesSorted[j].second)->indices.at(verticesSorted[j].first)=currentSize;
        int neig=0;
        
        for (int k = std::max(0, j-maxNeighbors);k<std::min((int)verticesSorted.size(),j+maxNeighbors);k++)
        {
            if (neig>maxNeighbors) break;
            if (k==j) continue;
            if (rootFaces.at(verticesSorted[k].second)->indices.at(verticesSorted[k].first)!=-1) continue;
            glm::dvec2 v1 =rootFaces.at(verticesSorted[k].second)->polarCoords.at(verticesSorted[k].first);
            glm::dvec2 v2 = rootFaces.at(verticesSorted[j].second)->polarCoords.at(verticesSorted[j].first);
            if (glm::length2(v1-v2)<std::numeric_limits<double>::epsilon())
            {
                rootFaces.at(verticesSorted[k].second)->indices.at(verticesSorted[k].first) = currentSize;
                normal+=rootFaces.at(verticesSorted[k].second)->GetNormal();
                neig++;
            }
        }
        //This corrects seams. It leads to other unintended artifacts, so it is currently disabled.
//        if (neig<2)
//        {
//            vvec3 disp = rootFaces.at(verticesSorted[j].second)->vertices[verticesSorted[j].first] - rootFaces.at(verticesSorted[j].second)->GetCenter();
//            newVertices[currentSize].SetPosition(newVertices[currentSize].GetPosition() + static_cast<vfloat>(0.1) * disp);
//        }
//        printf("%f %f\n",
//               rootFaces.at(verticesSorted[j].second)->polarCoords[0].x,rootFaces.at(verticesSorted[j].second)->polarCoords[0].y);
        normal = glm::normalize(normal);
        newVertices[currentSize].SetNormal(normal);
    }
    
    for (Face* f:rootFaces)
    {
        if (f->indices[0]!=-1 && f->indices[1]!=-1 && f->indices[2]!=-1)
        {
            newIndices.push_back(f->indices[0]);
            newIndices.push_back(f->indices[1]);
            newIndices.push_back(f->indices[2]);
        }
    }
#else
    for (Face& f : faces)
        recursiveUpdate(f, 0, 0, 0, player, newVertices, newIndices);
#endif
//    if (getPlayerDisplacementSquared(player)>displacementThreshold) return;
    {
        std::ofstream stream(resourcePath() + performanceOutput, std::ios::out | std::ios::app);
        stream << rootFaces.size() << "," << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t).count() <<"\n";
    }
    if (!closed)
    {
        std::lock_guard<std::mutex> lock(renderMutex);
        vertices = newVertices;
        indices = newIndices;
    }
}


void Planet::buildBaseMesh()
{
    //The first part of this function, which generates a list of coordinates of icosahedron vertices, is not mine.
    //I used code on a forum or website (unfortunately cannot find it again)
    vvec3 icosahedron[12];
    
    vvec2 icosahedronPolar[12];
    
    //reference angle for icosahedron vertices in radians -- used to calculate Cartesian coordinates of vertices
    double theta = 26.56505117707799 * M_PI / 180.0;
    
    double sine = std::sin(theta);
    double cosine = std::cos(theta);
    
    icosahedron[0] = vvec3(0.0f, 0.0f, -1.0f)*Radius; //bottom vertex
    //upper pentagon
    int i;
    double phi;
    for (i = 1, phi=M_PI/5.; i < 6; ++i,phi+=2.*M_PI/5.) {
        icosahedron[i] = vvec3(cosine * std::cos(phi), cosine * std::sin(phi), -sine)*Radius;
    }
    
    //lower pentagon
    for (i = 6, phi=0.; i < 11; ++i, phi+=2.*M_PI/5.) {
        icosahedron[i] = vvec3(cosine * std::cos(phi), cosine * std::sin(phi), sine)*Radius;
    }
    
    icosahedron[11] = vvec3(0.0, 0.0, 1.0)*Radius; // top vertex
    
    for (int i = 0; i<12;i++)
    {
        double len = glm::length(icosahedron[i]);
        icosahedronPolar[i] = glm::dvec2(std::acos(icosahedron[i].z / len), std::atan(icosahedron[i].y/icosahedron[i].x));
    }
    
    int faceIndices[] = {
        0,2,1,  0,3,2,  0,4,3,  0,5,4,  0,1,5,
        1,2,7,  2,3,8,  3,4,9,  4,5,10, 5,1,6,
        1,7,6,  2,8,7,  3,9,8,  4,10,9, 5,6,10,
        6,7,11, 7,8,11, 8,9,11, 9,10,11,10,6,11,
    };
    //generate 20 icosahedron faces
    for (int i = 0; i<20;i++)
    {
        faces.push_back(Face(nullptr, icosahedron[faceIndices[3 * i + 0]],
                             icosahedron[faceIndices[3 * i + 1]],
                             icosahedron[faceIndices[3 * i + 2]],
                             icosahedronPolar[faceIndices[3 * i + 0]],
                             icosahedronPolar[faceIndices[3 * i + 1]],
                             icosahedronPolar[faceIndices[3 * i + 2]]));
    }
}

void Planet::Draw()
{
    
    atmosphere.Position = static_cast<glm::vec3>(Position);
    setUniforms();
    time+=MainGame_SDL::ElapsedMilliseconds/10.;
    
    //adjust opengl rendering mode according to CurrentRenderMode -- wireframe or solid
    switch (CurrentRenderMode)
    {
        case RenderMode::SOLID:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case RenderMode::WIRE:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
    }
    
    unsigned int indsize, vertsize;
    GetIndicesVerticesSizes(indsize, vertsize);
    if (prevVerticesSize!=vertsize)
    {
        std::lock_guard<std::mutex> lock(renderMutex);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indsize, &indices[0], GL_DYNAMIC_DRAW);
        prevVerticesSize=vertsize;
    }
//    glDisable(GL_DEPTH_TEST);
//    glManager.Programs[1].Use();
//    atmosphere.Draw();
    
    if (vertsize >0)
    {
        glManager.Programs[0].Use();
//        glEnable(GL_DEPTH_TEST);
        std::lock_guard<std::mutex> lock(renderMutex);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);
    }
}

void Planet::UpdatePhysics(double timeStep)
{
    PhysicsObject::UpdatePhysics(timeStep);
    RotationMatrix=glm::rotate(vmat4(), Angle, glm::normalize(AngularVelocity));
    RotationMatrixInv=glm::inverse(RotationMatrix);//glm::rotate(vmat4(), -Angle, glm::normalize(AngularVelocity));
    Angle+=static_cast<vfloat>(timeStep) * glm::length(AngularVelocity);
    player.Camera.PlanetRotation=Angle;
}

void Planet::setUniforms()
{
    std::lock_guard<std::mutex> lock(renderMutex);
//    glManager.Programs[1].Use();
//    atmosphere.SetUniforms(glManager, *this);
//
//    glManager.Programs[1].SetFloat("fCameraHeight", len);
//    glManager.Programs[1].SetFloat("fCameraHeight2", len*len);
//    glManager.Programs[1].SetVector3("v3CameraPos", GetPlayerDisplacement());
//    
//    glManager.Programs[1].SetVector3("v3LightPos", glm::vec3(10*sin(angle), 10*cos(angle),0.0));
////    glManager.Programs[1].SetMatrix4("transformMatrix", glm::value_ptr(player.Camera.GetTransformMatrix()));
//    glManager.Programs[1].SetMatrix4("modelViewMatrix", glm::value_ptr(player.Camera.GetViewMatrix()));
//    glManager.Programs[1].SetMatrix4("projectionMatrix", glm::value_ptr(player.Camera.GetProjectionMatrix()));
    glManager.Programs[0].Use();
    glManager.Programs[0].SetFloat("time",time);
//    SeaLevel=-1;
//    SeaLevel=0.01;
    PlanetInfo.Radius = static_cast<float>(Radius);
    PlanetInfo.transformMatrix = player.Camera.GetTransformMatrix()*glm::translate(vmat4(), static_cast<vvec3>(Position))*RotationMatrix;
    glManager.UpdateBuffer("planet_info", &PlanetInfo, sizeof(PlanetInfo));
//    std::cout << "t: " << time << std::endl;
    
    glManager.Programs[0].SetVector3("sunDir", glm::vec3(RotationMatrixInv * glm::vec4(0, 1,0.0,1.0)));
//    player.Camera.PlanetRotation = CurrentRotationMode==RotationMode::ROTATION ? time*ROTATION_RATE : 0.0;
}