#version 410 core

#ifdef VERTEX_DOUBLE
#define vfloat double
#define vmat4 dmat4
#define vmat3 dmat3
#define vvec4 dvec4
#define vvec3 dvec3
#define vvec2 dvec2
#else
#define vfloat float
#define vmat4 mat4
#define vmat3 mat3
#define vvec4 vec4
#define vvec3 vec3
#define vvec2 vec2
#endif

layout (location = 0) in vvec3 vertexPos;
layout (location = 1) in vvec2 textureCoord;
layout (location = 2) in vvec3 normal;

layout(packed) uniform planet_info
{
    vec4 coldWaterColor;
    vec4 warmWaterColor;
    vec4 deepWaterColor;
    vec4 landColor;
    vec4 beachColor;
    vec4 mountainColor;
    vmat4 transformMatrix;
    float seaLevel;
    float specularity;
    float radius;
};

uniform float time;

out float height;
out float latitude;
out vec3 fragNormal;
out vec2 uv;

const vfloat waveAmplitude = 0.00001;
const float waveFrequency = 10.;
const vfloat waveNumber = 10000.;
uniform float aspectRatio;

void main()
{
    height = float(length(vertexPos))-radius;
    latitude = float(vertexPos.z);
    fragNormal = vec3(normal);
    vfloat mult =(1. + waveAmplitude * vfloat(cos(waveFrequency*time +float(waveNumber*(vertexPos.x + vertexPos.y + vertexPos.z)))));
    vfloat oceanInterp =clamp(10000.*(height-seaLevel), 0, 1);
    mult = mult - (mult - 1.)*oceanInterp;
    vec4 posvec =vec4(transformMatrix * (vec4(mult,mult,mult,1.0)*vec4(vertexPos,1.0)));
//    posvec.z = log2(posvec.z/256+1);
    uv = textureCoord;
    const float C = 0.01;
    posvec.z = (2 * log2(C*posvec.w + 1)) / log(C * 200. + 1) * posvec.w;
    
    gl_Position = posvec;
    
//    gl_Position.z = gl_Position.z;//log2(max(1e-6, 1.0 + gl_Position.z)) * 1.1;
}

