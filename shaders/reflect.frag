#version 330 core
out vec4 FragColor;

in vec3 WorldPos;
in vec3 Normal;

uniform samplerCube skybox;
uniform vec3 viewPos;
uniform float reflectivity;
uniform float alpha;

void main()
{
    vec3 I = normalize(WorldPos - viewPos);
    vec3 R = reflect(I, normalize(Normal));
    vec3 reflectedColor = texture(skybox, R).rgb;

    // Optionally use a base color or material color
    vec3 baseColor = vec3(0.5); // you can replace this with any material color

    // Blend between base color and reflection using the reflectivity slider
    vec3 finalColor = mix(baseColor, reflectedColor, reflectivity);
    FragColor = vec4(finalColor, alpha);
}
