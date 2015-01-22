#version 410 core
layout(location = 0) in vec3 vertexPosition;
uniform mat4 transformMatrix;
out vec3 incolor;
void main()
{
    incolor = vec3(1,0,0);
    gl_Position = transformMatrix * vec4(vertexPosition, 1.0);
}