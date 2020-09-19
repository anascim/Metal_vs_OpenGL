#version 330 core

in vec3 Normal;
in vec3 WorldPos;

out vec4 FragColor;

uniform vec3 cameraPos;

// Light
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;
uniform vec3 lightDirection;

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
    vec3 ambient = material.ambient * lightAmbient;

    vec3 lightDir = normalize(lightDirection);
    float nDotL = max(dot(Normal, -lightDir), 0.0f);
    vec3 diffuse = nDotL * material.diffuse * lightDiffuse;

    vec3 r = reflect(lightDir, Normal);
    vec3 camDir = normalize(cameraPos - WorldPos);
    float vDotR = max(dot(camDir, r), 0.0f);
    vec3 specular = pow(vDotR, material.shininess) * material.specular * lightSpecular;

    vec3 color = ambient + diffuse + specular;
    FragColor = vec4(color, 1.0f);
}
