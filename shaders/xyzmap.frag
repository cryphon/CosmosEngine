#version 330 core

in vec3 Normal;
out vec4 FragColor;

void main() {
    vec3 norm = normalize(Normal);
    vec3 baseColor = norm * 0.5 + 0.5; // Normal visualization
    FragColor = vec4(baseColor, 1.0);
}

