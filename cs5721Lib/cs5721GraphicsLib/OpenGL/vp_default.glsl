varying vec2 texCoord;

void main(void)
{
    texCoord = vec2(gl_MultiTexCoord0.s, gl_MultiTexCoord0.t);
    gl_Position = ftransform();
}