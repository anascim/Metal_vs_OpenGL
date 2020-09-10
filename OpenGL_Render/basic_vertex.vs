#version 330 core
layout (location = 0) in vec3 aPos; // "a" for "attribute"
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 WorldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   Normal = normalize(vec3(model * vec4(aNormal,0.0))); // correction for world space
    //a better way, but more computationaly expensive:
   //Normal = mat3(transpose(inverse(model))) * aNormal;
   WorldPos = vec3(model * vec4(aPos, 1.0));
   gl_Position = projection * view * model * vec4(aPos, 1.0);
}

