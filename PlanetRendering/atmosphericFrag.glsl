#version 410 core

in vec3 color;
out vec4 outcolor;

void main()
{
    outcolor = vec4(color,1.0);
}