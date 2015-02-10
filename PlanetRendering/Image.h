//
//  Image.h
//
//  Created by Christian on 12/27/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//
#pragma once
#include<vector>
#include<OpenGL/gl3.h>
#include "glm/glm.hpp"
class Image
{
public:
    Image(int index, GLsizei width, GLsizei height, std::vector<unsigned char>& pixels);
    const unsigned WIDTH, HEIGHT;
    std::vector<unsigned char> pixels;
    GLuint Texture;
    inline glm::vec3 GetFloatPixel(int i) const;
private:
};

glm::vec3 Image::GetFloatPixel(int i) const
{
    return glm::vec3(pixels[4*i], pixels[4*i+1], pixels[4*i+2])*(1.0f/256);
}