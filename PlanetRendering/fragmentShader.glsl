#version 410 core

//uniform float time;
out vec4 color;
in float height;
in vec3 fragNormal;
const float seaLevel=0.415;//225;

const vec4 waterColor = vec4(24./255.,51./255.,115./255.,1.0);
const vec4 landColor = vec4(52./255., 110./255., 53./255.,1.0);
const vec4 beachColor = vec4(204./255.,194./255.,120./255.,1.0);
uniform vec3 sunDir = vec3(1.0,0.0,0.0);

void main()
{
//    float interp=0;
    float interp = clamp(10000.*(height-seaLevel), 0, 1);
    color = waterColor - (waterColor - beachColor)*interp;
    interp = clamp(2000.*(height-seaLevel),0,1);
    color = color - (color - landColor)*interp;
    interp = clamp(10.*(height-seaLevel),0,1);
    interp*=interp*interp*interp*interp*interp*10000.;
    //color = color - (color - vec4(1,1,1,1))*interp;
    float lightness = clamp(dot(sunDir, fragNormal),0,1);
    color*=lightness + pow(lightness,20);//vec4(fragNormal,1.0);
    
//    float t = 100000.*(coord.x*coord.y*coord.z*coord.x*coord.y) + 0/100.;
//    color = vec4(sin(t),sin(t+1.),sin(t+2.),1.0);
}