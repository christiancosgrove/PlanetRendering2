//
//  Image.cpp
//  SupportVectorMachines
//
//  Created by Christian on 12/27/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//

#include "Image.h"

Image::Image(int index, size_t width, size_t height, std::vector<unsigned char>& _pixels) : WIDTH(width), HEIGHT(height), pixels(_pixels)
{
    //load texture
    glGenTextures(1, &Texture);
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glBindTexture(GL_TEXTURE_2D, 0);
}