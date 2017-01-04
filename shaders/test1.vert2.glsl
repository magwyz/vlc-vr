varying vec2 vTexCoord;
varying vec4 vColor;

attribute vec3 vPosition;
attribute vec2 MultiTexCoord0;


void main(void)
{
   vTexCoord = MultiTexCoord0.xy;
   vColor = gl_Color;
   gl_Position = gl_ModelViewProjectionMatrix * vec4(vPosition, 1.0);
}
