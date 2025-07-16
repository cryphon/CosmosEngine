#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

uniform sampler2D texture1;
uniform bool use_texture;

void main()
{
    vec3 finalColor;
    if (use_texture)
        finalColor = texture(texture1, texCoord).rgb;
    else
        finalColor = color;

    FragColor = vec4(finalColor, 1.0);
}

