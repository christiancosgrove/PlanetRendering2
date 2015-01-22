//
//  TextureManager.h
//  PlanetRendering
//
//  Created by Christian Cosgrove on 1/19/15.
//  Copyright (c) 2015 Christian. All rights reserved.
//
#pragma once
#include "Image.h"
#include <string>
class TextureManager
{
public:
    TextureManager();
    void LoadImage(const std::string& location);
private:
    std::vector<Image> images;
};