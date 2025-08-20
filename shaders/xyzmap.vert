#version 330 core
#include "glsl/common.glsl"

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;

uniform mat4 model;

void main() {
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    Normal = normalMatrix * aNormal;

    gl_Position = uProjection * uView * model * vec4(aPos, 1.0);
}

