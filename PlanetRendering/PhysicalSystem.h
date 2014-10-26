//
//  PhysicalSystem.h
//  PlanetRendering
//
//  Created by Christian Cosgrove on 10/23/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//
#pragma once
#include "PhysicsObject.h"
#include <vector>

class PhysicalSystem
{
public:
    PhysicalSystem(double g, double timeStep);
    const double GRAVITATIONAL_CONSTANT;
    double TimeStep;
    inline void AddObject(PhysicsObject& object);
    void Update();
protected:
    std::vector<PhysicsObject*> objects;
    
};

void PhysicalSystem::AddObject(PhysicsObject &object)
{
    objects.push_back(&object);
}