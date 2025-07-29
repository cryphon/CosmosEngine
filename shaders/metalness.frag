#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform float metalness; // 0.0 for dielectric, 1.0 for metal

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // normal, light and view directions
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseColor = objectColor * (1.0 - metalness * 0.95);
    vec3 diffuse = diff * lightColor * diffuseColor;

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    
    vec3 specularColor = mix(vec3(1.0), objectColor, metalness);
    float specularStrength = 0.5;
    vec3 specular = specularStrength * spec * lightColor * specularColor;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}

