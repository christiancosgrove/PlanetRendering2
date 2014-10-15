#version 410 core

layout (location = 0) in dvec4 vertexPos;
layout (location = 1) in dvec3 normal;

uniform dmat4 transformMatrix;

out float height;
out vec3 fragNormal;
const float seaLevel=0.415;//225;


void main()
{
    height = float(length(vertexPos)-1.0);
    fragNormal = vec3(normal);
    
    gl_Position = vec4(transformMatrix * vertexPos);// 0.1*sin(time/10.));
}