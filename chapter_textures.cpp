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

void drawTriangleTexture() {

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

    data = stbi_load("./images/wall.jpg", &width, &height, &numChannels, 0);

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

    // end: 加载纹理

    
    Shader mShader("./shaders/textures.vs", "./shaders/textures.frag");



    float vertices[] = {
            // vertex pos         // tex coords
            -0.5f, -0.5f,  0.0f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.0f,  1.0f, 0.0f,
             0.0f,  0.7f,  0.0f,  0.5f, 1.0f
        };

    

    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(glWindow.window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture);
        
        mShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

}

void drawRectangleTexture() {
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

    data = stbi_load("./images/wall.jpg", &width, &height, &numChannels, 0);

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

    // end: 加载纹理

    
    Shader mShader("./shaders/textures.vs", "./shaders/textures.frag");



    float vertices[] = {
        // vertex pos         // tex coords
        -0.5f, -0.5f,  0.0f,  0.0f, 0.0f, // left-bottom
         0.5f, -0.5f,  0.0f,  1.0f, 0.0f, // right-bottom
         0.5f,  0.5f,  0.0f,  1.0f, 1.0f, // right-top
        -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, // left-top
    };

    unsigned int indices[] = {
        0, 3, 2,
        2, 1, 0
    };

    

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(glWindow.window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture);
        
        mShader.use();
        glBindVertexArray(VAO);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
    }


    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}