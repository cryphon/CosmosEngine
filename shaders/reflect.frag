#version 330 core

#include "glsl/common.glsl"

out vec4 FragColor;

in vec3 WorldPos;
in vec3 Normal;

uniform samplerCube skybox;
uniform float reflectivity;
uniform float alpha;

void main()
{
    vec3 I = normalize(WorldPos - uViewPos.xyz);
    vec3 R = reflect(I, normalize(Normal));
    vec3 reflectedColor = texture(skybox, R).rgb;

    vec3 baseColor = vec3(0.5);

    // Blend between base color and reflection using the reflectivity slider
    vec3 finalColor = mix(baseColor, reflectedColor, reflectivity);
    FragColor = vec4(finalColor, alpha);
}
