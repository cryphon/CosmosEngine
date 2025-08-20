#version 330 core

#include "glsl/common.glsl"

layout (location = 0) in vec3 aPos;

uniform mat4 model;

void main() {
    gl_Position = uProjection * uView * model * vec4(aPos, 1.0);
}

