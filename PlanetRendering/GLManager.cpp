//
//  GLManager.cpp
//  PlanetRendering
//
//  Created by Christian on 9/28/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#include "GLManager.h"
#include <fstream>
#include <vector>

GLProgram::GLProgram(std::string fragName, std::string vertName) : programID(glCreateProgram()), fragmentShader(CompileShader(fragName, GL_FRAGMENT_SHADER)), vertexShader(CompileShader(vertName, GL_VERTEX_SHADER))
{
    //link program
    GLint result = GL_FALSE;
    int infoLogLength;
    
    fprintf(stdout, "Linking program\n");
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);
    
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> errorMessage(infoLogLength);
    glGetProgramInfoLog(programID, infoLogLength, NULL, &errorMessage[0]);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLManager::GLManager(std::string fragName, std::string vertName) : Program(fragName, vertName)
{
    Program.Use();
    initGL();
}

void GLManager::initGL()
{
//    glDisable(GL_CULL_FACE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDepthRange(0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}
//compile an OpenGL shader
GLuint GLProgram::CompileShader(std::string shaderName, GLenum type)
{
    GLuint shaderId = glCreateShader(type);
    
    std::string code;
    //read file with an ifstream
    std::ifstream stream(shaderName, std::ios::in);
    if (stream.is_open())
    {
        std::string line = "";
        while (std::getline(stream, line))
            code+="\n"+line;
        stream.close();
    }
    
    GLint result = GL_FALSE;
    int infoLogLength;
    
    //compile shader and list any errors in console
    printf("Compiling shader %s\n", shaderName.c_str());
    const char* src = code.c_str();
    glShaderSource(shaderId, 1, &src, 0);
    glCompileShader(shaderId);
    
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> errorMessage(infoLogLength);
    glGetShaderInfoLog(shaderId, infoLogLength, NULL, &errorMessage[0]);
    fprintf(stdout, "%s\n", &errorMessage[0]);
    
    return shaderId;
}