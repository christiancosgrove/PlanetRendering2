#version 410 core

//uniform float time;
out vec4 color;
in float height;
in float latitude;
in vec3 fragNormal;
uniform float seaLevel;
//const float seaLevel=0.415;//225;

const vec4 coldWaterColor = vec4(0.0945,0.2011,0.45,1.0);
const vec4 warmWaterColor = vec4(0.0867,0.3336,0.51,1.0);
const vec4 deepWaterColor = vec4(0.0462, 0.0983, 0.22,1.0);
const vec4 landColor = vec4(52./255., 110./255., 53./255.,1.0);
const vec4 beachColor = vec4(204./255.,194./255.,120./255.,1.0);
uniform vec3 sunDir = vec3(1.0,0.0,0.0);

const float specularity = 0.1;

void main()
{
    float interp=1 - abs(latitude);
    interp*=interp*interp;
    interp = 1-interp;
    vec4 waterColor = warmWaterColor - (warmWaterColor - coldWaterColor) * interp;
    vec4 deepWaterColor = waterColor/2.;
    
    interp = clamp(10000.*(height-seaLevel), 0, 1);
    color = waterColor - (waterColor - beachColor)*interp;
    interp = clamp((seaLevel - height)*100.0, 0, 1);
    color = color - (color - deepWaterColor) * interp;
    interp = clamp(2000.*(height-seaLevel),0,1);
    color = color - (color - landColor)*interp;
    interp = clamp(10.*(height-seaLevel),0,1);
    interp*=interp*interp*interp*interp*interp*10000.;
    //color = color - (color - vec4(1,1,1,1))*interp;WD
    float lightness = clamp(dot(sunDir, fragNormal),0,1);
    color*=0.5 + 0.5 * (lightness + specularity * pow(lightness,20));//vec4(fragNormal,1.0);
    
//    float t = 100000.*(coord.x*coord.y*coord.z*coord.x*coord.y) + 0/100.;
//    color = vec4(sin(t),sin(t+1.),sin(t+2.),1.0);
}