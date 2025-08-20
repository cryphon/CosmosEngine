#version 330 core
#include "glsl/common.glsl"

// Per-object (classic uniform for now)
uniform mat4 model;

// Vertex inputs
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoord;

// Varyings
out VS_OUT {
    vec3 WorldPos;
    vec3 WorldNormal;
    vec2 UV;
} v;

void main()
{
    vec4 wPos = model * vec4(aPos, 1.0);
    v.WorldPos   = wPos.xyz;
    v.WorldNormal = mat3(transpose(inverse(model))) * aNormal;
    v.UV         = aTexCoord;

    gl_Position = uProjection * uView * wPos;
}

