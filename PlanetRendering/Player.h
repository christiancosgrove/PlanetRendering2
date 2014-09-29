//
//  Player.h
//  PlanetRendering
//
//  Created by Christian on 9/28/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#ifndef __PlanetRendering__Player__
#define __PlanetRendering__Player__
#include <SFML/Graphics.hpp>
#include "glm/glm.hpp"

class Player
{
public:
    glm::vec3 Position;
    Player();
    Player(glm::vec3 pos);
    void Update();
    glm::mat4 GetViewMatrix();
};

#endif /* defined(__PlanetRendering__Player__) */
