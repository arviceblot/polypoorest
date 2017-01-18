varying vec3 normal;

void main(void)
{
   // gl_FragColor = vec4(abs(normal), 1.0);
   vec4 abnormal = vec4(abs(normal), 1.0);
   gl_FragColor = abnormal.rgra;
}
