#version 330 core
out vec4 FragColor;

uniform vec4 uColor;
uniform sampler2D uTexture;
uniform vec2 uTexRepetition;

in vec2 TexCoords;

void main()
{
   FragColor = texture(uTexture, TexCoords * uTexRepetition) * uColor;
}