//
//  Shader.cpp
//  LearnOpenGL-xcode
//
//  Created by zhiyun on 2020/8/17.
//  Copyright © 2020 zu. All rights reserved.
//

#include "RenderProgram.h"

using namespace std;

RenderProgram::RenderProgram() {
    
}

RenderProgram::RenderProgram(const char *vertexPath, const char *fragmentPath)
{
    compile(vertexPath, fragmentPath);
}

RenderProgram::~RenderProgram() {
    glDeleteProgram(ID);
}

void RenderProgram::use()
{
    glUseProgram(ID);
}

bool RenderProgram::isReady() {
    return ID > 0;
}

bool RenderProgram::compile(const char *vertexPath, const char *fragmentPath) {
    string vertexCode;
    string fragmentCode;

    ifstream vShaderFile;
    ifstream fShaderFile;

    vShaderFile.exceptions(ifstream::badbit | ifstream::failbit);
    fShaderFile.exceptions(ifstream::badbit | ifstream::failbit);

    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (ifstream::failure e)
    {
        cout << "Failed to read shader files" << endl;
        return false;
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
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        // cout << "Error when compiling vertex shader: " << infoLog << endl;
        printf("Error when compiling vertex shader: %s\n", infoLog);
        return false;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        // cout << "Error when compiling fragment shader: " << infoLog << endl;
        printf("Error when compiling fragment shader: %s\n", infoLog);
        return false;
    }

    ID = glCreateProgram();

    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);

    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        cout << "Error when compiling program: " << infoLog << endl;
        return false;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return ID > 0;
}

void RenderProgram::setBool(const string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void RenderProgram::setInt(const string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void RenderProgram::setFloat(const string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void RenderProgram::setVec2(const string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void RenderProgram::setVec2(const string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void RenderProgram::setVec3(const string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void RenderProgram::setVec3(const string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);;
}

void RenderProgram::setVec4(const string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void RenderProgram::setVec4(const string &name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void RenderProgram::setMat2(const string &name, const glm::mat2 &value) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
void RenderProgram::steMat3(const string &name, const glm::mat3 &value) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
void RenderProgram::setMat4(const string &name, const glm::mat4 &value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}