#version 320 core
layout (location = 0) in vec3 vertexPos;

in float height;

void main()
{
//    gl_Position.xyz=vertexPos;
    //    gl_Position.w=1.0;
     height = length(vertexPos);
    gl_Position = gl_ModelViewProjectionMatrix * vec4(vertexPOs,1.0);
}