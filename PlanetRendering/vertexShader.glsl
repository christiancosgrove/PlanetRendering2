#version 410 core
layout (location = 0) in vec4 vertexPos;

uniform mat4 transformMatrix;

out float height;

void main()
{
//    gl_Position.xyz=vertexPos;
    //    gl_Position.w=1.0;
     height = length(vertexPos)-1.0;
    gl_Position = transformMatrix * vertexPos;
}