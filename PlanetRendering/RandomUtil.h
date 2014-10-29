//
//  RandomUtil.h
//  PlanetRendering
//
//  Created by Christian Cosgrove on 10/29/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//
#pragma once
#include <random>

inline float randFloat()
{
    return (float)rand() / RAND_MAX;
}