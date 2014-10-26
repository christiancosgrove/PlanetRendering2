#version 410 core

in vec3 color;
in vec3 secondaryColor;
out vec4 outcolor;

void main()
{
    outcolor = vec4(color,1.0);
}