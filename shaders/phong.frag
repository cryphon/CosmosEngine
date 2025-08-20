#version 330 core

#include "glsl/common.glsl"

uniform vec3 uPhongDiffuse;   
uniform vec3 uPhongSpecular;  
uniform float uPhongShininess;

in VS_OUT {
    vec3 WorldPos;
    vec3 WorldNormal;
    vec2 UV;
} f;

out vec4 FragColor;

void main()
{
    vec3 N = normalize(f.WorldNormal);
    vec3 L = normalize(uLightPos.xyz - f.WorldPos);   // works now
    vec3 V = normalize(uViewPos.xyz  - f.WorldPos);   // works now
    vec3 H = normalize(L + V);

    float NdotL = max(dot(N, L), 0.0);

    vec3 diffuse  = uPhongDiffuse * NdotL * uLightColor.rgb;
    float spec    = pow(max(dot(N, H), 0.0), uPhongShininess);
    vec3 specular = uPhongSpecular * spec * uLightColor.rgb;

    vec3 color = diffuse + specular;
    FragColor = vec4(color, 1.0);
}




