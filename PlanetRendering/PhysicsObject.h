//
//  PhysicsObject.h
//  PlanetRendering
//
//  Created by Christian Cosgrove on 10/23/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//
#pragma once
#include "glm/glm.hpp"
#include "typedefs.h"

class PhysicsObject
{
public:
    
    PhysicsObject(glm::dvec3 position, double mass);
    PhysicsObject(glm::dvec3 position, glm::dvec3 velocity, double mass);
    glm::dvec3 Position;
    glm::dvec3 Velocity;
    glm::dvec3 NetForce;
    glm::dvec3 NextNetForce;
    double Energy;
    double Mass;
    
    virtual void UpdatePhysics(double timeStep);
    void ApplyForce(glm::dvec3 force, glm::dvec3 nextNetForce);
    void ApplyForce(glm::dvec3 force);
    
private:
    
};
