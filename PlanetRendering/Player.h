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
class Player
{
public:
    Player(int windowWidth, int windowHeight);
    Player(glm::vec3 pos, int windowWidth, int windowHeight);
    void Update();
    Camera Camera;
    vfloat DistFromSurface;
private:
};

#endif /* defined(__PlanetRendering__Player__) */
