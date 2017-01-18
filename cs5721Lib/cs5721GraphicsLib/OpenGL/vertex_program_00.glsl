uniform vec3 color_change;

void main(void)
{
    gl_FrontColor = gl_Color;
    gl_Position = ftransform();

    // gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}