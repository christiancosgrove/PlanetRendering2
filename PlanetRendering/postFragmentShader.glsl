#version 410 core

uniform sampler2D renderTexture;

out vec4 color;

vec2 resolution = vec2(2560,1440);
void main()
{
    color = texture(renderTexture, gl_FragCoord.xy/resolution);//vec4(1.,0.0,0.0,1.);
}