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
uniform bool use_texture;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 baseColor = Color;

    if (use_texture) {
        vec3 texColor = texture(uTexture, TexCoord).rgb;
        baseColor *= texColor;
    }


    FragColor = vec4((0.2 + diffuse) * baseColor, 1.0);
}


