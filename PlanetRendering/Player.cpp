//
//  Player.cpp
//  PlanetRendering
//
//  Created by Christian on 9/28/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#include "Player.h"
#include "glm/gtc/matrix_transform.hpp"


Player::Player() {}
Player::Player(glm::vec3 pos) : Position(pos) {}

void Player::Update()
{
    const float speed = 0.01f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        Position.y+=speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        Position.y-=speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        Position.x+=speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        Position.x-=speed;
}

glm::mat4 Player::GetViewMatrix()
{
    return glm::translate(glm::mat4(), Position);
}