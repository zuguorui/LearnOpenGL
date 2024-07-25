#pragma once

#include <stdlib.h>
#include <iostream>

#include "default_gl_include.h"
#include "glm/glm.hpp"

class ComputeProgram {
public:
    ComputeProgram();
    ComputeProgram(const char *shaderPath);
    ~ComputeProgram();

    void use();

    bool isReady();

    bool compile(const char *shaderPath);

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec2(const std::string &name, float x, float y) const;

    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;

    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w) const;

    void setMat2(const std::string &name, const glm::mat2 &value) const;
    void steMat3(const std::string &name, const glm::mat3 &value) const;
    void setMat4(const std::string &name, const glm::mat4 &value) const;
    
    uint32_t ID = 0;
};