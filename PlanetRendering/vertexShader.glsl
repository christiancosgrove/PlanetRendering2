#version 410 core

layout (location = 0) in dvec4 vertexPos;

uniform dmat4 transformMatrix;

out float height;
out vec3 coord;


void main()
{
//    gl_Position.xyz=vertexPos;
    //    gl_Position.w=1.0;
    height = float(length(vertexPos)-1.0);
    coord = vec3(vertexPos);
    gl_Position = vec4(transformMatrix * vertexPos);// 0.1*sin(time/10.));
}