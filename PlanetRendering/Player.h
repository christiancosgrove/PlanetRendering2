//
//  Player.h
//  PlanetRendering
//
//  Created by Christian on 9/28/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#ifndef __PlanetRendering__Player__
#define __PlanetRendering__Player__
#include "glm/glm.hpp"
#include "Camera.h"
#include "typedefs.h"
#include "PhysicsObject.h"
#include <thread>
//This class contains functions for controlling the camera using the mouse and keyboard.
class Player : public PhysicsObject
{
public:
    Player(int windowWidth, int windowHeight);
    Player(glm::vec3 pos, int windowWidth, int windowHeight);
    void Update(double timeStep);
    Camera Camera;
    vfloat DistFromSurface;
    std::mutex PlayerMutex;
private:
};

#endif /* defined(__PlanetRendering__Player__) */
