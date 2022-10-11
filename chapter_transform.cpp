
#include "chapter_transform.h"
#include "default_gl_include.h"
#include "GLWindow.h"
#include "Shader.h"

#include "stb_include.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <stdlib.h>

using namespace std;

void testTransform() {

    GLWindow glWindow = GLWindow::Builder().setTitle("transform").build();

    // begin: 加载纹理
    int width, height, numChannels;
    uint8_t *data = nullptr;

    stbi_set_flip_vertically_on_load(true);

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

    Shader mShader("./shaders/transform.vs", "./shaders/mix_textures.frag");

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    mShader.use();
    mShader.setInt("texture0", 0);
    mShader.setInt("texture1", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    while (!glfwWindowShouldClose(glWindow.window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        
        mShader.use();

        glm::mat4 trans(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, 0.5f, 0.0f));
        trans = glm::rotate(trans, (float)(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));

        mShader.setMat4("transform", trans);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glDeleteTextures(1, &texture0);
    glDeleteTextures(1, &texture1);


}