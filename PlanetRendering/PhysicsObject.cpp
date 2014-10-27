//
//  PhysicsObject.cpp
//  PlanetRendering
//
//  Created by Christian Cosgrove on 10/23/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//
#include "PhysicsObject.h"


PhysicsObject::PhysicsObject(glm::dvec3 position, double mass) : Position(position), Mass(mass), Velocity(glm::vec3(0.01,0.0,0.0))
{}

PhysicsObject::PhysicsObject(glm::dvec3 position, glm::dvec3 initialVelocity, double mass) : Position(position), Velocity(initialVelocity), Mass(mass)
{}

void PhysicsObject::UpdatePhysics(double timeStep)
{
    //perform Velocity Verlet integration
    //see http://en.wikipedia.org/wiki/Verlet_integration#Velocity_Verlet
    auto currAcceleration = NetForce / Mass;
    auto newPos = Position + Velocity * timeStep + 0.5 * currAcceleration * timeStep * timeStep;
    auto newAcceleration = NextNetForce * (1./Mass);
    auto newVelocity = Velocity + 0.5 * timeStep * (currAcceleration + newAcceleration);
    Velocity = newVelocity;
    Position = newPos;
    NetForce=glm::dvec3();
    NextNetForce=glm::dvec3();
}

void PhysicsObject::ApplyForce(glm::dvec3 force, glm::dvec3 nextForce)
{
    NetForce+=force;
    NextNetForce+=nextForce;
}
void PhysicsObject::ApplyForce(glm::dvec3 force)
{
    ApplyForce(force, force);
}