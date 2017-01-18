void main(void)
{
  // gl_Color is the color vertex attribute you defined for your
  // object. You can pass it to the fragment shader using built-in
  // OpenGL state variables:
  gl_FrontColor = gl_Color;
 
  // All vertex programs must set the special shader output variable,
  // gl_Position. This is the OpenGL canonical volume space
  // position. The gl_ModelViewProjectionMatrix combines the M_local,
  // M_view, and M_projection matrices together for you.
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
