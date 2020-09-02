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
   Normal = mat3(transpose(inverse(model))) * aNormal; // correction for world space
   WorldPos = vec3(model * vec4(aPos, 1.0));
   gl_Position = projection * view * model * vec4(aPos, 1.0);
}
