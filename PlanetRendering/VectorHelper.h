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

glm::vec3 Normalize(glm::vec3 v)
{
    float len = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    return glm::vec3(v.x/len,v.y/len,v.z/len);
}

#endif
