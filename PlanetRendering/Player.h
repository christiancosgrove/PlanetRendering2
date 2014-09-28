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


class Player
{
public:
    sf::Vector3f Position;
    Player();
    Player(sf::Vector3f pos);
};

#endif /* defined(__PlanetRendering__Player__) */
