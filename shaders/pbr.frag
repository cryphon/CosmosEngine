#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

uniform vec3 viewPos;

uniform sampler2D uAlbedoMap;
uniform bool useAlbedoMap;

void main() {
    vec3 albedo = useAlbedoMap ? texture(uAlbedoMap, TexCoords).rgb : vec3(1.0, 0.0, 1.0);
    
    // normalize the normal vector
    vec3 N = normalize(Normal);

    // view direction
    vec3 V = normalize(viewPos - FragPos);

    // Light direction (toward the light) -- fixed in world/view space 
    vec3 L = normalize(vec3(-0.5, 1.0, -0.3)); // directional light
    
    // diffuse shading
    float diff = max(dot(N, L), 0.0);

    vec3 color = albedo * diff;
    FragColor = vec4(color, 1.0);
}


