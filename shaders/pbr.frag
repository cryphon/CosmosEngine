#version 330 core

#include "glsl/common.glsl"

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;
in vec3 Tangent;
in vec3 Bitangent;


uniform vec3 uAlbedo;
uniform float uMetallic;
uniform float uRoughness;
uniform float uTiling;

uniform sampler2D uAlbedoMap;
uniform bool useAlbedoMap;

uniform sampler2D uNormalMap;
uniform bool useNormalMap;

uniform sampler2D uRoughnessMap;
uniform bool useRoughnessMap;

uniform sampler2D uMetallicMap;
uniform bool useMetallicMap;

uniform sampler2D uAOMap;
uniform bool useAOMap;

uniform sampler2D uDisplacementMap;
uniform bool useDisplacementMap;

const float PI = 3.14159265359;
const float parallaxStrength = 0.02;


vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float distributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return a2 / denom;
}

float geometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;

    float denom = NdotV * (1.0 - k) + k;
    return NdotV / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = geometrySchlickGGX(NdotV, roughness);
    float ggx2 = geometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

void main() {

    vec2 tiledUV = TexCoords * uTiling;


    vec2 displacedUV = tiledUV;
    if(useDisplacementMap) {
        // parallax mapping
        float height = texture(uDisplacementMap, tiledUV).r;

        vec3 viewDir = normalize(uViewPos.xyz - FragPos);
        vec2 offset = viewDir.xy * (height - 0.5) * parallaxStrength;
        displacedUV += offset;
    }



    vec3 lightColor = vec3(1.0, 0.95, 0.9); // slightly warm white
    float lightIntensity = 5.0;

    vec3 radiance = lightColor * lightIntensity;
    vec3 albedo = useAlbedoMap ? pow(texture(uAlbedoMap, displacedUV).rgb, vec3(2.2)) : uAlbedo;
    vec3 normalMap = useNormalMap ? texture(uNormalMap, tiledUV).rgb : Normal;    
    normalMap = normalMap * 2.0 - 1.0; // convert from [0, 1] to [-1, 1]
    float roughness = useRoughnessMap ? texture(uRoughnessMap, tiledUV).r : uRoughness;
    float metallic = useMetallicMap ? texture(uMetallicMap, tiledUV).r : uMetallic;
    float ao = useAOMap ? texture(uAOMap, tiledUV).r : 1.0;
    

    mat3 TBN = mat3(normalize(Tangent), normalize(Bitangent), normalize(Normal)); // make sure T, B, N are in the same space as lighting
    // normalize the normal vector
    vec3 N = normalize(Normal);
    if (useNormalMap) {
        vec3 map = texture(uNormalMap, tiledUV).rgb * 2.0 - 1.0;
        N = normalize(TBN * map);
}    // view direction
    vec3 V = normalize(uViewPos.xyz - FragPos);

    // Light direction (toward the light) -- fixed in world/view space 
    vec3 L = normalize(vec3(-0.5, 1.0, -0.3)); // directional light

    // Half vector
    vec3 H = normalize(V + L);

    float D = distributionGGX(N, H, roughness);


    float G = geometrySmith(N, V, L, roughness);

    // Fresnel reflectance at normal incidence
    vec3 F0 = vec3(0.04);   // dielectric base reflectivity
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);


    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);

    vec3 numerator = D * F * G;
    float denominator = 4.0 * NdotL * NdotV + 0.001; // avoid division by zero
    vec3 specular = numerator / denominator;
    
    // diffuse shading
    float diff = max(dot(N, L), 0.0);


    // ao shading with dark gray fallback ambient
    vec3 ambientColor = vec3(0.03); // dark gray fallback ambient
    vec3 ambient = ao * ambientColor * albedo;


    
    // PBR
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    vec3 diffuse = kD * albedo / PI;
    vec3 Lo = (diffuse + specular) * radiance * NdotL;

    vec3 finalColor = ambient + Lo;
    vec3 color = albedo * diff;
    // Final output color (gamma correction manually)
    FragColor = vec4(pow(finalColor, vec3(1.0/2.2)), 1.0);
}


