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

glm::dvec3 Normalize(glm::dvec3 v)
{
    double len = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    return glm::dvec3(v.x/len,v.y/len,v.z/len);
}

#endif
