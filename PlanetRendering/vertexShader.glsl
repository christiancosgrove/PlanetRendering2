#version 410 core

#ifdef VERTEX_DOUBLE
#define vfloat double
#define vmat4 dmat4
#define vmat3 dmat3
#define vvec4 dvec4
#define vvec3 dvec3
#else
#define vfloat float
#define vmat4 mat4
#define vmat3 mat3
#define vvec4 vec4
#define vvec3 vec3
#endif

layout (location = 0) in vvec3 vertexPos;
layout (location = 1) in vvec3 normal;

uniform vmat4 transformMatrix;
uniform float time;

out float height;
out float latitude;
out vec3 fragNormal;
uniform float seaLevel=0.01;

const vfloat waveAmplitude = 0.0000001;
const float waveFrequency = 10.;
const vfloat waveNumber = 100000.;
uniform vvec3 origin;
uniform float aspectRatio;


void main()
{
    height = float(length(vertexPos - origin))-1.;
    latitude = float(vertexPos.z);
    fragNormal = vec3(normal);
    vfloat mult =(1. + waveAmplitude * vfloat(cos(waveFrequency*time +float(waveNumber*(vertexPos.x + vertexPos.y + vertexPos.z)))));
    vfloat oceanInterp =clamp(10000.*(height-seaLevel), 0, 1);
    mult = mult - (mult - 1.)*oceanInterp;
    gl_Position = vec4(transformMatrix * (vec4(mult,mult,mult,1.0)*vec4(vertexPos,1.0)));
}

