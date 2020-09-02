#version 330 core

in vec3 Normal;
in vec3 WorldPos;

out vec4 FragColor;

uniform vec3 cameraPos;

// Light
uniform vec3 lightColor;
uniform vec3 lightPos;

// Material
struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

void main()
{
    vec3 ambient = material.ambient;

    vec3 lightDir = normalize(WorldPos - lightPos);
    float nDotL = max(dot(Normal, -lightDir), 0.0f);
    vec3 diffuse = nDotL * material.diffuse;

    vec3 reflection = reflect(lightDir, Normal);
    vec3 camDir = normalize(cameraPos - WorldPos);
    float vDotR = max(dot(camDir, reflection), 0.0f);
    vec3 specular = pow(vDotR, material.shininess) * material.specular;

    vec3 color = (ambient + diffuse + specular) * lightColor;
    FragColor = vec4(color, 1.0f);
}
