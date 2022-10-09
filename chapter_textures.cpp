#include "chapter_textures.h"
#include "default_gl_include.h"
#include "GLWindow.h"
#include "Shader.h"
#include "util.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <iostream>
#include <stdlib.h>

using namespace std;

void testTexture() {

    GLWindow glWindow = GLWindow::Builder().setTitle("texture").build();

    // begin: 设置顶点着色器
    string vsCode = readShaderProgram("./shaders/hello_triangle.vs");
    if (vsCode.size() == 0) {
        cout << "read vertex shader code failed" << endl;
        return;
    }

    const char *vsCodeChar = vsCode.c_str();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vsCodeChar, nullptr);
    glCompileShader(vertexShader);

    if (!checkShaderCompileStatus(vertexShader)) {
        return;
    }
    // end: 设置顶点着色器

    // begin: 设置片段着色器
    string fragCode = readShaderProgram("./shaders/hello_triangle.frag");
    if (fragCode.size() == 0) {
        return;
    }

    const char *fragCodeChar = fragCode.c_str();
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragCodeChar, nullptr);
    glCompileShader(fragmentShader);
    if (!checkShaderCompileStatus(fragmentShader)) {
        return;
    }

    // end: 设置片段着色器

    // begin: 设置着色器program

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (!checkProgramLinkStatus(shaderProgram)) {
        return;
    }

    // end: 设置着色器program

    // begin: 加载纹理
    int width, height, numChannels;
    uint8_t *data = nullptr;

    data = stbi_load("./images/wawll.jpg", &width, &height, &numChannels, 0);

    if (!data) {
        cout << "load image failed" << endl;
        return;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    



    float texCoords[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.5f, 1.0f
    };

    

    GLuint VAO, VBO;
}