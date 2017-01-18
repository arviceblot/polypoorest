varying vec3 normal;

void main(void)
{
    normal = gl_Normal;

    gl_FrontColor = gl_Color;
    gl_Position = ftransform();

    // gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}