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
#include <vector>
#include <unordered_map>
//This class encapsulates OpenGL shader programs to simplify shader loading
class GLProgram
{
public:
    GLuint fragmentShader, vertexShader, programID;
    
    GLProgram(const std::string& fragName, const std::string& vertName);
    //This function reads from a local file and compiles it into an OpenGL shader
    static GLuint CompileShader(const std::string& shaderName, GLenum type);
    inline void Use();
    inline void SetMatrix4(std::string name, const GLfloat* value);
    inline void SetMatrix4(std::string name, const GLdouble* value);
    inline void SetFloat(std::string name, GLfloat value);
    inline void SetVector3(std::string name, const GLfloat* value);
    inline void SetVector3(std::string name, const glm::vec3& value);
    inline void SetVector3(std::string name, const GLdouble* value);
    inline void SetVector3(std::string name, const glm::dvec3& value);
};

void GLProgram::Use()
{
    glUseProgram(programID);
}
void GLProgram::SetMatrix4(std::string name, const GLfloat *value)
{
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, value);
}
void GLProgram::SetMatrix4(std::string name, const GLdouble *value)
{
    glUniformMatrix4dv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, value);
}
void GLProgram::SetFloat(std::string name, GLfloat value)
{
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}
void GLProgram::SetVector3(std::string name, const GLfloat *value)
{
    glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, value);
}
void GLProgram::SetVector3(std::string name, const glm::vec3 &value)
{
    SetVector3(name, glm::value_ptr(value));
}
void GLProgram::SetVector3(std::string name, const GLdouble* value)
{
    glUniform3dv(glGetUniformLocation(programID, name.c_str()), 1, value);
}
void GLProgram::SetVector3(std::string name, const glm::dvec3& value)
{
    SetVector3(name, glm::value_ptr(value));
}



//this class further abstracts the GLProgram class, allowing for additional OpenGL configuration
class GLManager
{
public:
    std::vector<GLProgram> Programs;
    GLManager(const std::string& fragName, const std::string& vertName);
    int AddProgram(const std::string fragmentShaderName, const std::string& vertexShaderName);
    void AddUniformBuffer(const std::string& name, std::size_t size, std::initializer_list<GLuint> programs);
    void UpdateBuffer(const std::string& name, const void* value, std::size_t size);
private:
    std::unordered_map<std::string, GLuint> uniformBuffers;
    //some OpenGL configuration
    void initGL();
};

#endif /* defined(__PlanetRendering__GLManager__) */
