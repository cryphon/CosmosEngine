#version 330 core
layout (location = 0) in vec3 aPos;

layout(std140) uniform PerView {
    mat4 uView;
    mat4 uProjection;
    vec4 uViewPos;     // xyz used
    vec4 uLightPos;    // xyz used
    vec4 uLightColor;  // rgb used
    vec4 uParams;      // x=reflectivity, y=alpha
};

void main() {
    gl_Position = uProjection * uView * vec4(aPos, 1.0);
}

