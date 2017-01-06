#version 120

attribute vec3 vPosition;
attribute vec2 MultiTexCoord0;


void main(void)
{
   gl_TexCoord[0] = vec4(MultiTexCoord0.xy, 0, 0);
   gl_Position = vec4(vPosition, 1.0);
}

