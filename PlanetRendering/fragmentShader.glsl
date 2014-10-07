//out vec4 color;
varying float height;
const float seaLevel=0.42;

void main()
{
    
    gl_FragColor = (height < seaLevel) ? vec4(0.,0.,1.,1.) : vec4(52./255., 110./255., 53./255.,1.0);
}