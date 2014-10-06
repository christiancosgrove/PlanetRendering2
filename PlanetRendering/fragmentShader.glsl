//out vec4 color;
varying float height;
void main()
{
    gl_FragColor = vec4((height-1.0)*20.,0,0,1);
}