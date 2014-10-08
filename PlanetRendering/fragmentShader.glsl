#version 410 core

out vec4 color;
in float height;
const float seaLevel=0.42;

void main()
{
    
    color = (height < seaLevel) ? vec4(0.,0.,1.,1.) : vec4(52./255., 110./255., 53./255.,1.0);
}