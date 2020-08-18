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

using namespace std;

class Shader
{
public:
    Shader(const char *vertexPath, const char *fragmentPath);
    
    void use();
    void setBool(const string &name, bool value) const;
    void setInt(const string &name, int value) const;
    void setFloat(const string &name, float value) const;
    
    uint32_t ID;
    
};

#endif /* Shader_hpp */
