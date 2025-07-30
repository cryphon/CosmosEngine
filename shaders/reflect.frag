#version 330 core
out vec4 FragColor;

in vec3 WorldPos;
in vec3 Normal;

uniform samplerCube skybox;
uniform vec3 viewPos;
uniform float reflectivity;
uniform float alpha;
uniform vec3 baseColor;

void main()
{
    vec3 I = normalize(WorldPos - viewPos);
    vec3 R = reflect(I, normalize(Normal));
    vec3 reflectedColor = texture(skybox, R).rgb;


    // Blend between base color and reflection using the reflectivity slider
    vec3 finalColor = mix(baseColor, reflectedColor, reflectivity);
    FragColor = vec4(finalColor, alpha);
}
