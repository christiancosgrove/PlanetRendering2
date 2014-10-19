#version 410 core

layout (location = 0) in dvec4 vertexPos;
layout (location = 1) in dvec3 normal;

uniform dmat4 transformMatrix;
uniform float time;

out float height;
out float latitude;
out vec3 fragNormal;
const float seaLevel=0.415;//225;

const double waveAmplitude = 0.0000001;
const float waveFrequency = 0.01;
const double waveNumber = 100000.;


void main()
{
    height = float(length(vertexPos))-1.;
    latitude = float(vertexPos.z);
    fragNormal = vec3(normal);
    double mult =(1. + waveAmplitude * double(cos(waveFrequency*time +float(waveNumber*(vertexPos.x + vertexPos.y + vertexPos.z)))));
    double oceanInterp =clamp(10000.*(height-seaLevel), 0, 1);
    mult = mult - (mult - 1.)*oceanInterp;
    gl_Position = vec4(transformMatrix * (vec4(mult,mult,mult,1.0)*vertexPos));
}