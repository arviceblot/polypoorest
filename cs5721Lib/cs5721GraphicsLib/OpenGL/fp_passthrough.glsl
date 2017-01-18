uniform sampler2DRect texId0;
uniform sampler2DRect texId1;

varying vec2 texCoord;

void main(void)
{
   vec3 val0 = vec3( texture2DRect(texId0, texCoord) );
   vec3 val1 = vec3( texture2DRect(texId1, texCoord) );  // not used, but you could

   if ( sin(3.1459 * gl_FragCoord.x / 125.0) > 0.0 ) {
      gl_FragColor = vec4(1.0 - val0.r, 1.0 - val0.g, 1.0 - val0.b, 1.0);
   }
   else
      gl_FragColor = vec4(val0.r, val0.g, val0.b, 1.0);
}
