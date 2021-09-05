#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexture;

out vec3 FragPos;
out vec3 FragNormal;
out vec2 FragTexture;

uniform mat4 model, view, projection;
uniform mat4 normal;

void main()
{
    gl_Position = projection * view * model * vec4(vPos, 1.0);

    FragPos = vec3(model * vec4(vPos, 1.0));
    FragNormal =  mat3(normal) * vNormal;
    FragTexture = vTexture;
}
