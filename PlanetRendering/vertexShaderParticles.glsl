#version 330 core

layout(location=0) in vec3 vertexPosition;
uniform mat4 transformMatrix;

void main()
{
    gl_Position = transformMatrix * vec4(vertexPosition, 1);
    
}