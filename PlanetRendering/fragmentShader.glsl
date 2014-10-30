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

//uniform float time;
out vec4 color;
in float height;
in float latitude;
in vec3 fragNormal;

layout(packed) uniform planet_info
{
    vec4 coldWaterColor;
    vec4 warmWaterColor;
    vec4 deepWaterColor;
    vec4 landColor;
    vec4 beachColor;
    vmat4 transformMatrix;
    float seaLevel;
    float specularity;
    float radius;
};

//const float seaLevel=0.415;//225;

uniform vec3 sunDir = vec3(1.0,0.0,0.0);



void main()
{
    float interp=1 - abs(latitude);
    interp*=interp*interp;
    interp = 1-interp;
    vec4 waterColor = warmWaterColor - (warmWaterColor - coldWaterColor) * interp;
    vec4 deepWaterColor = waterColor/2.;
    float newSpec = specularity;
    
    interp = clamp(100000.*(height-seaLevel), 0, 1);
    color = waterColor - (waterColor - beachColor)*interp;
    interp = clamp((seaLevel - height)*100.0, 0, 1);
    color = color - (color - deepWaterColor) * interp;
    interp = clamp(2000.*(height-seaLevel),0,1);
    color = color - (color - landColor)*interp;
    newSpec*=interp;
    interp = clamp(10.*(height-seaLevel),0,1);
    interp*=interp*interp*interp*interp*interp*10000.;
    //color = color - (color - vec4(1,1,1,1))*interp;WD
    float lightness = clamp(dot(sunDir, fragNormal),0,1);
    color*=0.05 + 0.95 * (lightness + (1-newSpec) * pow(lightness,20));//vec4(fragNormal,1.0);
//    gl_FragDepth =  log2(gl_FragCoord.z/256+1);
//    float t = 100000.*(coord.x*coord.y*coord.z*coord.x*coord.y) + 0/100.;
//    color = vec4(sin(t),sin(t+1.),sin(t+2.),1.0);
}