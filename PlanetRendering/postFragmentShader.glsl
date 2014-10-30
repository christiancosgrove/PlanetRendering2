#version 410 core

uniform sampler2D renderTexture;

out vec4 color;

uniform vec2 resolution;
//const int blurAmount=4;
//#define PI 3.14159265f
void main()
{
//    vec2 dv = vec2(1.)/resolution;
//    vec4 currColor=vec4(0.0,0.0,0.0,1.0);
//    for (int dist = -blurAmount; dist<blurAmount;dist++)
//    {
//        for (int dist2 = -blurAmount; dist2<blurAmount;dist2++)
//        {
//            float normal = 1./(5*blurAmount * sqrt(2*PI))*exp((-dist*dist-dist2*dist2)/(2*25*blurAmount*blurAmount));
//            currColor+=normal*texture(renderTexture, gl_FragCoord.xy/resolution + vec2(dist,dist2)*dv);
//        }
//    }
    color = texture(renderTexture, gl_FragCoord.xy/resolution);//currColor;//blurAmount/blurAmount;
//    color = (texture(renderTexture, gl_FragCoord.xy/resolution)*4+
//             texture(renderTexture, gl_FragCoord.xy/resolution + dist*dv.x) +
//             texture(renderTexture, gl_FragCoord.xy/resolution + dist*dv.y) +
//             texture(renderTexture, gl_FragCoord.xy/resolution - dist*dv.x) +
//             texture(renderTexture, gl_FragCoord.xy/resolution - dist*dv.y))/8.;//vec4(1.,0.0,0.0,1.);
}