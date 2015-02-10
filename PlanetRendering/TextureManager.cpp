//
//  TextureManager.cpp
//  PlanetRendering
//
//  Created by Christian Cosgrove on 1/19/15.
//  Copyright (c) 2015 Christian. All rights reserved.
//

#include "TextureManager.h"
#include "picopng.h"
#include <fstream>

TextureManager::TextureManager() {}

//Efficient file loading, from
//http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
std::string get_file_contents(const char *filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize((size_t)in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    throw(errno);
}

void TextureManager::LoadImage(const std::string &location)
{
    
    std::vector<unsigned char> out;
    unsigned long width, height;
    
    std::string in=  get_file_contents(location.c_str());
    
    
    decodePNG(out, width, height, (const unsigned char*)&in[0], in.size());
    
    size_t index = images.size();
    images.push_back(Image(index, (GLsizei)width, (GLsizei)height, out));
}