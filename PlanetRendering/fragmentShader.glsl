#version 410 core

//uniform float time;
out vec4 color;
in float height;
in vec3 coord;
const float seaLevel=0.42225;

void main()
{
    float interp=0;
    //float interp = pow((height - seaLevel),2);
//    color = vec4(0.,0.,1.,1.) - (vec4(0.,0.,1.,1.) -vec4(52./255., 110./255., 53./255.,1.0)) *interp;
    float t = 100000.*(coord.x*coord.y*coord.z*coord.x*coord.y) + 0/100.;
    color = vec4(sin(t),sin(t+1.),sin(t+2.),1.0);
}