//
//  AABB.h
//  PlanetRendering
//
//  Created by Christian Cosgrove on 1/21/15.
//  Copyright (c) 2015 Christian. All rights reserved.
//
#pragma once
#include "glm/glm.hpp"
#include "typedefs.h"
#include <cmath>
#include <array>
struct AABB
{
    vvec2 origin, dimensions;
    static inline bool Intersection(AABB r1, AABB r2) {
        return ((std::abs(r1.origin.x - r2.origin.x)*2 < (r1.dimensions.x+r2.dimensions.x)) &&
                (std::abs(r1.origin.y - r2.origin.y)*2 < (r1.dimensions.y+r2.dimensions.y)));
    }
    AABB(std::array<vvec2,3>& vertices)
    {
        int corner = 0;
        if (vertices[1].x<vertices[corner].x && vertices[1].y<vertices[corner].y) corner=1;
        if (vertices[2].x<vertices[corner].x && vertices[2].y<vertices[corner].y) corner=2;
        origin=vertices[corner];
        dimensions=vvec2();
        
        for (int i = 0; i<3;i++)
        {
            if (i==corner) continue;
            if (origin.x+dimensions.x<vertices[i].x) dimensions.x=vertices[i].x-origin.x;
            if (origin.y+dimensions.y<vertices[i].y) dimensions.y=vertices[i].y-origin.y;
        }
    }
    AABB(vfloat x, vfloat y, vfloat width, vfloat height) : origin(x,y), dimensions(width,height) {}
    
};