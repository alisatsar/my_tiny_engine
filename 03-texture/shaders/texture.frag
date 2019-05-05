#version 300 es
precision mediump float;
in vec3 ourColor;
in vec2 TexCoord;
uniform sampler2D outTexture;
void main()
{
fragColor = texture(outTexture, TexCoord);
}
