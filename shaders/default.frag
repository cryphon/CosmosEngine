#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 Color;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform bool selected;

void main() {
    // Ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Texture color
    vec3 texColor = texture(uTexture, TexCoord).rgb;

    // Final color logic
    vec3 baseColor = Color * texColor;

    // Override with highlight color if selected
    if (selected) {
        baseColor = mix(baseColor, vec3(0.6, 0.6, 0.6), 0.6);
    }

    vec3 result = (ambient + diffuse) * baseColor;
    FragColor = vec4(result, 1.0);
}


