#version 330 core
out vec4 FragColor;

in vec3 WorldPos;
in vec3 Normal;

uniform samplerCube skybox;
uniform vec3 viewPos;

void main()
{
    vec3 I = normalize(WorldPos - viewPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = texture(skybox, R);
}
