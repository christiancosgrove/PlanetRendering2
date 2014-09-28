//
//  GLManager.h
//  PlanetRendering
//
//  Created by Christian on 9/28/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#ifndef __PlanetRendering__GLManager__
#define __PlanetRendering__GLManager__

#include <string>
#include <SFML/OpenGL.hpp>

class GLProgram
{
public:
    GLuint fragmentShader, vertexShader, programID;
    
    GLProgram(std::string fragName, std::string vertName);
    
    static GLuint CompileShader(std::string shaderName, GLenum type);
};


class GLManager
{
public:
    GLProgram Program;
    GLManager(std::string fragName, std::string vertName);
};

#endif /* defined(__PlanetRendering__GLManager__) */
