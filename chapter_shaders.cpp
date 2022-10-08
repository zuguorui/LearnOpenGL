#include "default_gl_include.h"
#include "GLWindow.h"
#include "chapter_shaders.h"
#include "util.h"
#include <iostream>

using namespace std;

void testShaders() {
    GLWindow glWindow = GLWindow::Builder().setTitle("shaders").build();

    if (!glWindow.success()) {
        cout << "failed to create window" << endl;
        return;
    }

    // begin: 设置顶点着色器
    string vsCode = readShaderProgram("./shaders/input_output.vs");
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
    string fragCode = readShaderProgram("./shaders/input_output.frag");
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

    float vertices[] = {
             0.0f,  0.0f,  0.0f,
             1.0f,  0.0f,  0.0f,
             0.5f,  0.7f,  0.0f
        };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(glWindow.window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}