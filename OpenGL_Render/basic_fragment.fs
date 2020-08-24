#version 330 core

//in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    FragColor = vec4(objectColor * lightColor, 1.0f);
}
