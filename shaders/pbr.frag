#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D uAlbedoMap;
uniform bool useAlbedoMap;

void main() {
    vec3 color = useAlbedoMap ? texture(uAlbedoMap, TexCoords).rgb : vec3(1.0, 0.0, 1.0);
    FragColor = vec4(color, 1.0);
}


