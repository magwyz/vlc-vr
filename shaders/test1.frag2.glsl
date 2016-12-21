uniform sampler2D myTexture;

varying vec2 vTexCoord;
varying vec4 vColor;


void main(void)
{
    if (vColor.a != 0.f)
        gl_FragColor = vColor;
    else
        gl_FragColor = texture2D(myTexture, vTexCoord);
}
