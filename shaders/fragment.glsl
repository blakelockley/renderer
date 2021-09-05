#version 330 core

in vec3 FragPos;
in vec3 FragNormal;
in vec2 FragTexture;

out vec4 FragColor;

uniform vec3 color;

void main()
{
    FragColor = vec4(color, 1.0f);
}