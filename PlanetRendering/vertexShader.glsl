//#version 410 core
//layout (location = 0) in vec4 vertexPos;

varying float height;

void main()
{
//    gl_Position.xyz=vertexPos;
    //    gl_Position.w=1.0;
     height = length(gl_Vertex);
    gl_Position = gl_ModelViewMatrix * gl_Vertex;
}