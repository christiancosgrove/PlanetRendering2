#version 320 core
out vec4 color;
in float height;
void main()
{
    gl_FragColor = vec4((height-0.5),0,0,1);
}