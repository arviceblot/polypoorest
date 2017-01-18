void main(void)
{
  // In the fragment shader, you are responsible for setting the
  // correct fragment color in the special Output Variable
  // gl_FragColor. Using gl_FrontColor here will perform the standard
  // barycentric interpolation between the front colors at each
  // vertex.
  gl_FragColor = gl_FrontColor;
}
