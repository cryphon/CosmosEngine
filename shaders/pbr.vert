#version 330 core

#include "glsl/common.glsl"

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


uniform sampler2D uDisplacementMap;
uniform float uDisplacementStrength;
uniform bool useDisplacementMap;

uniform float uTiling;

out vec3 WorldPos;
out vec3 WorldNormal;

void main() {

    vec4 w = model * vec4(aPos,1.0);
    WorldPos = w.xyz;
    WorldNormal = mat3(transpose(inverse(model))) * aNormal;
    Normal = mat3(transpose(inverse(model))) * aNormal;

    Tangent = mat3(model) * aTangent;
    Bitangent = mat3(model) * aBitangent;

    TexCoords = aTexCoord;

    vec2 tiledUV = TexCoords * uTiling; // Same tiling factor used in fragment shader
    float displacement = 0.0;
    if(useDisplacementMap) {
        displacement = texture(uDisplacementMap, tiledUV).r;
    }

    // apply displacement conditionally
    vec3 displacedPos = aPos;
    if(useDisplacementMap) {
        displacedPos += aNormal * (displacement - 0.5) * uDisplacementStrength;
    }

    gl_Position = uProjection * uView * model * vec4(displacedPos, 1.0);
    FragPos = vec3(model * vec4(displacedPos, 1.0));
}
