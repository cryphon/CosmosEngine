#version 330 core

in vec3 Normal;
out vec4 FragColor;

void main() {
    vec3 norm = normalize(Normal);
    FragColor = vec4(norm * 0.5 + 0.5, 1.0); // Map [-1, 1] to [0, 1]
}

