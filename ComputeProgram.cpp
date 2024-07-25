
#include "ComputeProgram.h"
#include <cstdio>
#include <sstream>
#include <istream>
#include <fstream>
#include <stdio.h>

using namespace std;

ComputeProgram::ComputeProgram() {

}

ComputeProgram::ComputeProgram(const char *shaderPath) {
    compile(shaderPath);
}

ComputeProgram::~ComputeProgram() {
    if (ID > 0) {
        glDeleteProgram(ID);
        ID = 0;
    }
}

void ComputeProgram::use() {
    glUseProgram(ID);
}

bool ComputeProgram::isReady() {
    return ID > 0;
}

bool ComputeProgram::compile(const char *shaderPath) {
    string shaderCode;
    ifstream shaderFile;
    shaderFile.exceptions(ifstream::badbit | ifstream::failbit);
    try {
        shaderFile.open(shaderPath);
        stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();

        shaderFile.close();
        shaderCode = shaderStream.str();
    } catch (ifstream::failure e) {
        cout << "failed to read shader file, path = " << shaderPath << endl;
        return false;
    }

    const char *shaderCodeCStr = shaderCode.c_str();

    GLuint shader;
    GLint success;
    char infoLog[512];

    shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader, 1, &shaderCodeCStr, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        printf("Error when compile compute shader: %s\n", infoLog);
        return false;
    }

    ID = glCreateProgram();
    glAttachShader(ID, shader);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (success != GL_TRUE) {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        printf("Error when link compute program: %s\n", infoLog);
        ID = 0;
        return false;
    }

    glDeleteShader(shader);
    return ID > 0;
}

void ComputeProgram::setBool(const string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void ComputeProgram::setInt(const string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void ComputeProgram::setFloat(const string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ComputeProgram::setVec2(const string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void ComputeProgram::setVec2(const string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void ComputeProgram::setVec3(const string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void ComputeProgram::setVec3(const string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);;
}

void ComputeProgram::setVec4(const string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void ComputeProgram::setVec4(const string &name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void ComputeProgram::setMat2(const string &name, const glm::mat2 &value) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
void ComputeProgram::steMat3(const string &name, const glm::mat3 &value) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
void ComputeProgram::setMat4(const string &name, const glm::mat4 &value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
