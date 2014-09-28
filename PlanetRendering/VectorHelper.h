//
//  VectorHelper.h
//  PlanetRendering
//
//  Created by Christian on 9/28/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#ifndef PlanetRendering_VectorHelper_h
#define PlanetRendering_VectorHelper_h
#include <cmath>
#include <SFML/Graphics.hpp>

sf::Vector3f Normalize(sf::Vector3f v)
{
    float len = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    return sf::Vector3f(v.x/len,v.y/len,v.z/len);
}

#endif
