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

class GLProgram
{
public:
    GLuint fragmentShader, vertexShader, programID;
    
    GLProgram(std::string fragName, std::string vertName);
    
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
};


class GLManager
{
public:
    GLProgram Program;
    GLManager(std::string fragName, std::string vertName);
private:
    void initGL();
};

#endif /* defined(__PlanetRendering__GLManager__) */
