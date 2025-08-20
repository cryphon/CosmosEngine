#version 330 core
#include "glsl/common.glsl"

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

void main()
{
    TexCoords = aPos;

    // strip translation from view
    mat4 viewNoT = mat4(mat3(uView));
    gl_Position = uProjection * viewNoT * vec4(aPos, 1.0);
}


