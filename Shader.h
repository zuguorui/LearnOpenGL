//
//  Shader.hpp
//  LearnOpenGL-xcode
//
//  Created by zhiyun on 2020/8/17.
//  Copyright © 2020 zu. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

//#define GLEW_STATIC
//#include <GL/glew.h>

#include "glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdint>

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;

class Shader
{
public:
    Shader(const char *vertexPath, const char *fragmentPath);
    
    void use();

    void setBool(const string &name, bool value) const;
    void setInt(const string &name, int value) const;
    void setFloat(const string &name, float value) const;

    void setVec2(const string &name, const glm::vec2 &value) const;
    void setVec2(const string &name, float x, float y) const;

    void setVec3(const string &name, const glm::vec3 &value) const;
    void setVec3(const string &name, float x, float y, float z) const;

    void setVec4(const string &name, const glm::vec4 &value) const;
    void setVec4(const string &name, float x, float y, float z, float w) const;

    void setMat2(const string &name, const glm::mat2 &value) const;
    void steMat3(const string &name, const glm::mat3 &value) const;
    void setMat4(const string &name, const glm::mat4 &value) const;
    
    uint32_t ID;
    
};

#endif /* Shader_hpp */
