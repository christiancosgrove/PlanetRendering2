//
//  PhysicalSystem.cpp
//  PlanetRendering
//
//  Created by Christian Cosgrove on 10/23/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#include "PhysicalSystem.h"
#include "glm/glm.hpp"
#include <fstream>
PhysicalSystem::PhysicalSystem(double g, double timeStep, const std::string& resourcePath) : GRAVITATIONAL_CONSTANT(g), TimeStep(timeStep), RESOURCE_PATH(resourcePath)
{
    std::ofstream ostream(resourcePath + "energy.csv", std::ios::out);
}

void PhysicalSystem::Update()
{
    //Calculate forces before integration
    //This prevents order of objects from affecting physics.
    double totalEnergy = 0;
    for (PhysicsObject* object : objects)
    {
        glm::dvec3 force;
        glm::dvec3 force2;
        //calculate force (this is currently inefficient, with a complexity of O(n^2)).
        //This can be remedied through space partitioning
        for (PhysicsObject* object2 : objects)
        {
            if (object2->Mass*object->Mass<=std::numeric_limits<double>::epsilon()) continue;
            //Prevent double-counting
            if (object2==object) continue;
            //Calculate displacement vector
            glm::dvec3 disp = object->Position - object2->Position;
            
            glm::dvec3 disp2 = object->Position + object->Velocity*TimeStep - object2->Position - object2->Velocity*TimeStep;
            double r = (glm::length(disp));//*63781000;
            double r2 = (glm::length(disp2));//*63781000;
            //Apply Newton's Law of Gravitation to net force.
            force -= GRAVITATIONAL_CONSTANT * object->Mass * object2->Mass / (r * r * r) * disp;
            force2 -= GRAVITATIONAL_CONSTANT * object->Mass * object2->Mass / (r2 * r2 * r2) * disp2;
            object->Energy-=GRAVITATIONAL_CONSTANT * object->Mass *object2->Mass/ r;
        }
        object->ApplyForce(force, force2);
    }
    time+=TimeStep;
    if (steps%ENERGY_POLLING_INTERVAL==0) logInCSV(std::to_string(time) + "," + std::to_string(totalEnergy));
    //Perform Verlet integration for each body
    for (int i = 0; i<objects.size();++i)
        objects[i]->UpdatePhysics(TimeStep);
    
    steps++;
}

void PhysicalSystem::logInCSV(const std::string& output)
{
    std::ofstream ostream(RESOURCE_PATH + "energy.csv", std::ios::out | std::ios::app);
    ostream << output << std::endl;
}