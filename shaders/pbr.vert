#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;
layout(location = 3) in vec2 aTexCoord;
layout(location = 4) in vec3 aTangent;
layout(location = 5) in vec3 aBitangent;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out vec3 Tangent;
out vec3 Bitangent;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    Normal = mat3(transpose(inverse(model))) * aNormal;

    Tangent = mat3(model) * aTangent;
    Bitangent = mat3(model) * aBitangent;

    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoord;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}

