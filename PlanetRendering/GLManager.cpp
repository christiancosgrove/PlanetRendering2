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
#include "typedefs.h"

GLProgram::GLProgram(const std::string& fragName, const std::string& vertName) : programID(glCreateProgram()), fragmentShader(CompileShader(fragName, GL_FRAGMENT_SHADER)), vertexShader(CompileShader(vertName, GL_VERTEX_SHADER))
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

GLManager::GLManager(const std::string& fragName, const std::string& vertName)
{
    AddProgram(fragName, vertName);
    initGL();
}

void GLManager::initGL()
{
//    glDisable(GL_CULL_FACE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);
//    glDepthRange(-1,1);
}
//compile an OpenGL shader
GLuint GLProgram::CompileShader(const std::string& shaderName, GLenum type)
{
    GLuint shaderId = glCreateShader(type);
    
    std::string code;
    //read file with an ifstream
    std::ifstream stream(shaderName, std::ios::in);
    if (stream.is_open())
    {
        std::string line = "";
        while (std::getline(stream, line))
        {
            code+="\n"+line;
#ifdef VERTEX_DOUBLE
            if (type==GL_VERTEX_SHADER && line.substr(0,8)=="#version")
            {
                code+="\n#define VERTEX_DOUBLE";
            }
#endif
        }
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

int GLManager::AddProgram(const std::string fragmentShaderName, const std::string& vertexShaderName)
{
    int id = Programs.size();
    fprintf(stdout, "Compiling program %i\n", id);
    Programs.push_back(GLProgram(fragmentShaderName, vertexShaderName));
    return id;
}
#include <iostream>
void GLManager::AddUniformBuffer(const std::string& name, std::size_t size, std::initializer_list<GLuint> programs)
{
    GLuint ubo;
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    for (auto programIdx : programs)
    {
        GLuint program = Programs[programIdx].programID;
        glUseProgram(program);
        GLint index = glGetUniformBlockIndex(program, name.c_str());
        
        std::cout << index << std::endl;
        GLuint bindingPoint = 0;
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ubo);
        glUniformBlockBinding(program, index, bindingPoint);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    uniformBuffers[name]=ubo;
}

void GLManager::UpdateBuffer(const std::string& name, const void *value, std::size_t size)
{
    if (uniformBuffers.find(name)==uniformBuffers.end()) throw std::out_of_range("Bad access.");
    GLuint ubo = uniformBuffers[name];
    
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
//    GLvoid* location = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
//    memcpy(location, value, size);
//    glUnmapBuffer(GL_UNIFORM_BUFFER);
    glBufferData(GL_UNIFORM_BUFFER, size, value, GL_DYNAMIC_DRAW);
}