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
#include <OpenGL/gl3.h>
#include "glm/gtc/type_ptr.hpp"
//This class encapsulates OpenGL shader programs to simplify shader loading
class GLProgram
{
public:
    GLuint fragmentShader, vertexShader, programID;
    
    GLProgram(std::string fragName, std::string vertName);
    //This function reads from a local file and compiles it into an OpenGL shader
    static GLuint CompileShader(std::string shaderName, GLenum type);
    inline void Use() { glUseProgram(programID); }
    inline void SetMatrix4fv(std::string name, const GLfloat* value)
    {
        glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, value);
    }
    inline void SetMatrix4dv(std::string name, const GLdouble* value)
    {
        glUniformMatrix4dv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, value);
    }
    inline void SetFloat(std::string name, GLfloat value)
    {
        glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
    }
    inline void SetVector3fv(std::string name, const GLfloat* value)
    {
        glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, value);
    }
    inline void SetVector3fv(std::string name, const glm::vec3& value)
    {
        SetVector3fv(name, glm::value_ptr(value));
    }
};

//this class further abstracts the GLProgram class, allowing for additional OpenGL configuration
class GLManager
{
public:
    GLProgram Program;
    GLManager(std::string fragName, std::string vertName);
private:
    //some OpenGL configuration
    void initGL();
};

#endif /* defined(__PlanetRendering__GLManager__) */
