//
//  Shader.cpp
//  LearnOpenGL-xcode
//
//  Created by zhiyun on 2020/8/17.
//  Copyright © 2020 zu. All rights reserved.
//

#include "Shader.h"


Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    string vertexCode;
    string fragmentCode;
    
    ifstream vShaderFile;
    ifstream fShaderFile;
    
    vShaderFile.exceptions(ifstream::badbit | ifstream::failbit);
    fShaderFile.exceptions(ifstream::badbit | ifstream::failbit);
    
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        
        stringstream vShaderStream, fShaderStream;
        
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        
        vShaderFile.close();
        fShaderFile.close();
        
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (ifstream::failure e) {
        cout << "Failed to read shader files" << endl;
    }
    
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    
    GLuint vertex, fragment;
    GLint success;
    char infoLog[512];
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        cout << "Error when compiling vertex shader: " << infoLog << endl;
        return;
    }
    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        cout << "Error when compiling fragment shader: " << infoLog << endl;
        return;
    }
    
    ID = glCreateProgram();
    
    
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    
    glLinkProgram(ID);
    
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        cout << "Error when compiling program: " << infoLog << endl;
        return;
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
