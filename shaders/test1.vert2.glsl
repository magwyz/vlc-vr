varying vec2 vTexCoord;
varying vec4 vColor;


void main(void)
{
   vTexCoord = gl_MultiTexCoord0.xy;
   vColor = gl_Color;
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
