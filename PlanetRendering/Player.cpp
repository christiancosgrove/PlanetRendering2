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

#include <SDL2/SDL.h>
Player::Player(int windowWidth, int windowHeight) : Camera(windowWidth, windowHeight) {}
Player::Player(glm::vec3 pos, int windowWidth, int windowHeight) : Camera(windowWidth, windowHeight) {}

//void Player::Update() // SFML implementation
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
void Player::Update() //SDL implementation
{
    bool mouseFocus = true;
    double len =glm::length(Camera.Position)-1.0;
    double playerSpeed = std::min((std::exp2(len)-1.0)/100.0,0.0025);
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    double shiftSpeedFactor = (state[SDL_SCANCODE_LSHIFT]) ? 45.0 : 1.0;
    if (state[SDL_SCANCODE_W])
    {
        Camera.Position+=Camera.GetViewDirection() * playerSpeed * shiftSpeedFactor;
    }
    if (state[SDL_SCANCODE_S])
    {
        Camera.Position-=Camera.GetViewDirection() * playerSpeed * shiftSpeedFactor;
    }
    if (state[SDL_SCANCODE_A])
    {
        Camera.Position+=glm::dvec3(glm::dvec4(1.0, 0.0, 0.0, 1.0) * glm::eulerAngleXZ(Camera.XRotation, Camera.ZRotation)) * playerSpeed * shiftSpeedFactor;
    }
    if (state[SDL_SCANCODE_D])
    {
        Camera.Position-=glm::dvec3(glm::dvec4(1.0, 0.0, 0.0, 1.0) * glm::eulerAngleXZ(Camera.XRotation, Camera.ZRotation)) * playerSpeed * shiftSpeedFactor;
    }
    if (state[SDL_SCANCODE_SPACE])
    {
        Camera.Position-=glm::dvec3(glm::dvec4(0.0, 1.0, 0.0, 1.0) * glm::eulerAngleXZ(Camera.XRotation, Camera.ZRotation)) * playerSpeed * shiftSpeedFactor;
    }
//    if (state[SDL_SCANCODE_LSHIFT])
//    {
//        Camera.Position-=glm::dvec3(glm::dvec4(0.0, 0.0, 1.0, 1.0) * glm::eulerAngleXZ(Camera.XRotation, Camera.ZRotation)) * playerSpeed * shiftSpeedFactor;
//    }
    if (state[SDL_SCANCODE_LALT])
        mouseFocus = !mouseFocus;
    
    const float earthDiameter = 12756200.0;
    
    if (mouseFocus)
    {
        int x,y;
        SDL_GetRelativeMouseState(&x, &y);
        
        Camera.XRotation += (float) (y)/200;
        if (Camera.XRotation > 0) Camera.XRotation=0;
        if (Camera.XRotation<-M_PI) Camera.XRotation=-M_PI;
        Camera.ZRotation+=(float)(x)/200;
        std::cout << "Height above earth surface: " << len * earthDiameter << " m" <<std::endl;
    }
    
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
}