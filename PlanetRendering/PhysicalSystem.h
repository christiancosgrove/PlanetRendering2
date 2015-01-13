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
#include <string>
#include "glm/gtx/norm.hpp"

class PhysicalSystem
{
public:
    PhysicalSystem(double g, double timeStep, const std::string& resourcePath);
    const double GRAVITATIONAL_CONSTANT;
    double TimeStep;
    inline void AddObject(PhysicsObject& object);
    void Update();
protected:
    std::vector<PhysicsObject*> objects;
    void logInCSV(const std::string& output);
    double time;
    const int ENERGY_POLLING_INTERVAL=10;
    const std::string RESOURCE_PATH;
    int steps;
};

void PhysicalSystem::AddObject(PhysicsObject &object)
{
    objects.push_back(&object);
}