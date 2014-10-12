#version 410 core

//uniform float time;
out vec4 color;
in float height;
in vec3 coord;
const float seaLevel=0.415;//225;

const vec4 waterColor = vec4(24./255.,51./255.,115./255.,1.0);
const vec4 landColor = vec4(52./255., 110./255., 53./255.,1.0);

void main()
{
//    float interp=0;
    float interp = clamp(10000.*(height-seaLevel), 0, 1);
    color = waterColor - (waterColor - landColor)*interp;//(height > seaLevel) ? landColor : waterColor;//vec4(0.,0.,1.,1.) - (vec4(0.,0.,1.,1.) -vec4(52./255., 110./255., 53./255.,1.0)) *interp;
//    float t = 100000.*(coord.x*coord.y*coord.z*coord.x*coord.y) + 0/100.;
//    color = vec4(sin(t),sin(t+1.),sin(t+2.),1.0);
}