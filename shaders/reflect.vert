#version 330 core

#include "glsl/common.glsl"

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 WorldPos;
out vec3 Normal;

uniform mat4 model;

void main()
{
    vec4 worldPosition = model * vec4(aPos, 1.0);
    WorldPos = worldPosition.xyz;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = uProjection * uView * worldPosition;
}

