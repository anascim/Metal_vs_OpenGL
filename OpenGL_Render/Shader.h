#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include "glm.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

class Shader
{
    public:
        unsigned int ID;
        Shader(const char* vertexPath, const char* fragmentPath);
        void use();
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setFloat2(const std::string &name, glm::vec2 value) const;
        void setFloat3(const std::string &name, glm::vec3 value) const;
        void setFloat4(const std::string &name, glm::vec4 value) const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;
};

#endif // SHADER_H
