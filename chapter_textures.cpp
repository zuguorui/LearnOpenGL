#include "chapter_textures.h"
#include "default_gl_include.h"
#include "GLWindow.h"
#include "RenderProgram.h"
#include "util.h"

#include "default_stb_include.h"

#include <iostream>
#include <stdlib.h>

using namespace std;

void drawTriangleTexture() {

    GLWindow glWindow = GLWindow::Builder().setTitle("texture").build();

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

    
    RenderProgram mShader("./shaders/textures.vs", "./shaders/textures.frag");



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

    glDeleteTextures(1, &texture);

}

void drawRectangleTexture() {
    GLWindow glWindow = GLWindow::Builder().setTitle("texture").build();

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    // end: 加载纹理

    RenderProgram mShader("./shaders/textures.vs", "./shaders/textures.frag");

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

    glDeleteTextures(1, &texture);
}

void drawMixTexture() {
    GLWindow glWindow = GLWindow::Builder().setTitle("texture").build();

    // begin: 加载纹理
    int width, height, numChannels;
    uint8_t *data = nullptr;

    data = stbi_load("./images/wall.jpg", &width, &height, &numChannels, 0);

    if (!data) {
        cout << "load image failed" << endl;
        return;
    }

    GLuint texture0;
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    data = nullptr;

    data = stbi_load("./images/awesomeface.png", &width, &height, &numChannels, 0);

    if (!data) {
        cout << "load image failed" << endl;
        return;
    }

    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    data = nullptr;



    // end: 加载纹理

    RenderProgram mShader("./shaders/textures.vs", "./shaders/mix_textures.frag");

    float vertices[] = {
        // vertex pos         // tex coords
        -0.5f, -0.5f,  0.0f,  0.0f, 0.0f, // left-bottom
         0.5f, -0.5f,  0.0f,  1.0f, 0.0f, // right-bottom
         0.5f,  0.5f,  0.0f,  1.0f, 1.0f, // right-top
        -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, // left-top
    };

    unsigned int indices[] = {
        0, 3, 2,
        0, 1, 2
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

    mShader.use();
    mShader.setInt("texture0", 0);
    mShader.setInt("texture1", 1);



    while (!glfwWindowShouldClose(glWindow.window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        
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

    glDeleteTextures(1, &texture0);
    glDeleteTextures(1, &texture1);

}