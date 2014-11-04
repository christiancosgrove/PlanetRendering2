//
//  Player.cpp
//  PlanetRendering
//
//  Created by Christian on 9/28/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#include "Player.h"
#include "glm/gtc/matrix_transform.hpp"
#include <algorithm>
#include <iostream>
#include "MainGame_SDL.h"

#include <SDL2/SDL.h>
Player::Player(int windowWidth, int windowHeight) : Camera(windowWidth, windowHeight), DistFromSurface(10.0), PhysicsObject(glm::dvec3(20.1, 10.0, 10.0), 1.) {}
Player::Player(glm::vec3 pos, int windowWidth, int windowHeight) : Camera(windowWidth, windowHeight), DistFromSurface(10.0), PhysicsObject(glm::dvec3(20.1, 10.0, 10.0), 0.00001) {}

//void Player::Update() // SFML implementation (original)
//{
//    bool mouseFocus = true;
//    const float playerSpeed = 0.05f;
//    float shiftSpeedFactor = (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) ? 45.0f : 1.0f;
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
//    {
//        Camera.Position+=Camera.GetViewDirection() * playerSpeed;
//    }
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
//    {
//        Camera.Position-=Camera.GetViewDirection() * playerSpeed;
//    }
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
//    {
//        Camera.Position+=glm::vec3(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) * glm::eulerAngleXZ(Camera.XRotation, Camera.ZRotation)) * playerSpeed;
//    }
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
//    {
//        Camera.Position-=glm::vec3(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) * glm::eulerAngleXZ(Camera.XRotation, Camera.ZRotation)) * playerSpeed;
//    }
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
//        mouseFocus = !mouseFocus;
//    if (mouseFocus)
//    {
//        Camera.XRotation += (float)(sf::Mouse::getPosition().y - 540)/200;
//        if (Camera.XRotation>0)
//            Camera.XRotation=0;
//        if (Camera.XRotation<-M_PI)
//            Camera.XRotation=-M_PI;
//        Camera.ZRotation += (float)(sf::Mouse::getPosition().x - 960)/200;
//        sf::Mouse::setPosition(sf::Vector2<int>(960,540));
//    }
//}
void Player::Update(double timeStep) //SDL implementation -- updated
{
    bool mouseFocus = true;
    //vfloat len =glm::length(Camera.GetPosition())-1.0;
    vfloat playerSpeed = 0.0000001/timeStep;// * MainGame_SDL::ElapsedMilliseconds;// std::min((std::exp2(len)-1.)/100.0,0.0025);
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    vfloat shiftSpeedFactor = (state[SDL_SCANCODE_LSHIFT] ? 200.0 : 1.0) * (state[SDL_SCANCODE_LALT] ? 100. : 1.);
    if (state[SDL_SCANCODE_W])
    {
        Velocity+=(static_cast<glm::dvec3>(-Camera.GetViewDirection() * playerSpeed * shiftSpeedFactor));
    }
    if (state[SDL_SCANCODE_S])
    {
        Velocity+=(static_cast<glm::dvec3>(Camera.GetViewDirection() * playerSpeed * shiftSpeedFactor));
    }
    if (state[SDL_SCANCODE_A])
    {
        Velocity+=(static_cast<glm::dvec3>(-vvec3(vvec4(1.0, 0.0, 0.0, 1.0) * glm::eulerAngleXZ(Camera.XRotation, Camera.ZRotation)) * playerSpeed * shiftSpeedFactor));
    }
    if (state[SDL_SCANCODE_D])
    {
        Velocity+=(static_cast<glm::dvec3>(vvec3(vvec4(1.0, 0.0, 0.0, 1.0) * glm::eulerAngleXZ(Camera.XRotation, Camera.ZRotation)) * playerSpeed * shiftSpeedFactor));
    }
    if (state[SDL_SCANCODE_SPACE])
    {
        Velocity/=1.05;
//        Velocity+=(static_cast<glm::dvec3>(vvec3(vvec4(0.0, 1.0, 0.0, 1.0) * glm::eulerAngleXZ(Camera.XRotation, Camera.ZRotation)) * playerSpeed * shiftSpeedFactor));
    }
    if (state[SDL_SCANCODE_R])
        Velocity=glm::dvec3();
    if (state[SDL_SCANCODE_Q])
    {
        Camera.YRotation+=0.5;
    }
    if (state[SDL_SCANCODE_E])
    {
        Camera.YRotation-=0.5;
    }
    Camera.position = static_cast<vvec3>(Position);
    
    if (mouseFocus)
    {
        int x,y;
        SDL_GetRelativeMouseState(&x, &y);
        
        Camera.XRotation += (float) (y)/200;
        if (Camera.XRotation > 0) Camera.XRotation=0;
        if (Camera.XRotation<-M_PI) Camera.XRotation=-M_PI;
        Camera.ZRotation+=(float)(x)/200;
    }
    
}