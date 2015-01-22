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
in vec2 uv;
uniform sampler2D terrainTexture;
uniform sampler2D normalTexture;


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

//const float seaLevel=0.415;//225;

uniform vec3 sunDir = vec3(1.0,0.0,0.0);
const float ambientLight=0.1;

float rand(vec2 co);
const int numLevels=1;
const float textureScaling=10;

#define M_PI 3.1415926535897932384626433832795

void main()
{
    float interp=1 - abs(latitude);
    interp*=interp*interp;
    interp = 1-interp;
    vec4 waterColor = warmWaterColor - (warmWaterColor - coldWaterColor) * interp;
    vec4 deepWaterColor = waterColor/2.;
    float newSpec = specularity;
    
    interp = clamp(10000.*(height-seaLevel), 0, 1);
    color = waterColor - (waterColor - beachColor)*interp;
    interp = clamp((seaLevel - height)*100.0, 0, 1);
    color = color - (color - deepWaterColor) * interp;
    interp = clamp((height-seaLevel)*100., 0, 1);
    vec4 interpLand = landColor - (landColor-mountainColor)*interp;
    interp = clamp(2000.*(height-seaLevel),0,1);
    color = color - (color - interpLand)*interp;
    newSpec*=interp;
    interp = clamp(10.*(height-seaLevel),0,1);
    interp*=interp*interp*interp*interp*interp*10000.;
    //color = color - (color - vec4(1,1,1,1))*interp;WD
    vec2 newU = ((uv+vec2(1,1)))*0.5*textureScaling;
    
    vec3 norm=vec3(0,0,0);
    // + texture(normalTexture,newU).xyz + texture(normalTexture,newU*0.1).xyz + texture(normalTexture,newU*10).xyz;
    for (int i = 0; i<numLevels;i++)
        norm+=texture(normalTexture, newU / (1 << i)).xyz;
    norm=normalize(norm/numLevels+fragNormal);
    float lightness = clamp(dot(sunDir, norm),0,1);
    float mult=0;
    for (int i = 0; i<numLevels;i++)
        mult+=texture(terrainTexture, newU / (1 << i)).r;
    color*=mult/numLevels;
    color*=ambientLight + (1-ambientLight) * (lightness + (1-newSpec) * pow(lightness,20));//vec4(fragNormal,1.0);
//    gl_FragDepth =  log2(gl_FragCoord.z/256+1);
//    float t = 100000.*(coord.x*coord.y*coord.z*coord.x*coord.y) + 0/100.;
//    color = vec4(sin(t),sin(t+1.),sin(t+2.),1.0);
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}