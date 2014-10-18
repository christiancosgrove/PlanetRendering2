#version 410 core

layout (location = 0) in dvec4 vertexPos;
layout (location = 1) in dvec3 normal;

uniform dmat4 transformMatrix;
uniform float time;

out float height;
out vec3 fragNormal;
const float seaLevel=0.415;//225;


void main()
{
    height = float(length(vertexPos))-1.;
    fragNormal = vec3(normal);
    double mult =(1. + 0.000001 * double(cos(0.01*time +float(100000.*(vertexPos.x + vertexPos.y + vertexPos.z)))));
    double oceanInterp =clamp(10000.*(height-seaLevel), 0, 1);
    mult = mult - (mult - 1.)*oceanInterp;
    gl_Position = vec4(transformMatrix * (vec4(mult,mult,mult,1.0)*vertexPos));
}