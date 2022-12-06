#include "yuv2rgb.h"

#include "yuv_loader.h"
#include "default_gl_include.h"

using namespace std;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void yuv_to_rgb() {
    GLWindow glWindow = GLWindow::Builder().setSize(SCREEN_WIDTH, SCREEN_HEIGHT).setTitle("yuv2rgb").build();

    const int SRC_WIDTH = 1536;
    const int SRC_HEIGHT = 864;

    uint8_t **yuv = nullptr;

    int y_width = SRC_WIDTH;
    int y_height = SRC_HEIGHT;
    
    // yuv444p
    // const char *path = "assets/out-1536*864-444p.yuv";
    // int u_width = SRC_WIDTH;
    // int u_height = SRC_HEIGHT;
    // int v_width = SRC_WIDTH;
    // int v_height = SRC_HEIGHT;
    // bool success = load_yuv444p(path, SRC_WIDTH, SRC_HEIGHT, &yuv);
    
    // yuv420p
    // const char *path = "assets/out-1536*864-420p.yuv";
    // nv21
    const char *path = "assets/out-1536*864-nv21.yuv";
    int u_width = SRC_WIDTH / 2;
    int u_height = SRC_HEIGHT / 2;
    int v_width = SRC_WIDTH / 2;
    int v_height = SRC_HEIGHT / 2;
    // bool success = load_yuv420p(path, SRC_WIDTH, SRC_HEIGHT, &yuv);
    bool success = load_yuv420sp(path, SRC_WIDTH, SRC_HEIGHT, &yuv);


    if (!success) {
        cout << "load yuv failed" << endl;
        return;
    }

    GLuint tex_y;
    glGenTextures(1, &tex_y);

    glBindTexture(GL_TEXTURE_2D, tex_y);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, y_width, y_height, 0, GL_RED, GL_UNSIGNED_BYTE, yuv[0]);
    glGenerateMipmap(GL_TEXTURE_2D);

    GLuint tex_u;
    glGenTextures(1, &tex_u);
    glBindTexture(GL_TEXTURE_2D, tex_u);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, u_width, u_height, 0, GL_RED, GL_UNSIGNED_BYTE, yuv[1]);
    glGenerateMipmap(GL_TEXTURE_2D);

    GLuint tex_v;
    glGenTextures(1, &tex_v);
    glBindTexture(GL_TEXTURE_2D, tex_v);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, v_width, v_height, 0, GL_RED, GL_UNSIGNED_BYTE, yuv[2]);
    glGenerateMipmap(GL_TEXTURE_2D);

    free(yuv[0]);
    free(yuv[1]);
    free(yuv[2]);
    free(yuv);


    float vertices[] = {
        // vertex pos         // tex coords
        -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, // view left-bottom to tex left-top
         1.0f, -1.0f,  0.0f,  1.0f, 1.0f, // view right-bottom to tex right-top
         1.0f,  1.0f,  0.0f,  1.0f, 0.0f, // view right-top to tex right-bottom
        -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, // view left-top to tex left-bottom
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

    Shader mShader("./shaders/yuv_rgb.vs", "./shaders/yuv_rgb.frag");

    mShader.use();
    mShader.setInt("tex_y", 0);
    mShader.setInt("tex_u", 1);
    mShader.setInt("tex_v", 2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_y);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex_u);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, tex_v);



    while (!glfwWindowShouldClose(glWindow.window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        mShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glDeleteTextures(1, &tex_y);
    glDeleteTextures(1, &tex_u);
    glDeleteTextures(1, &tex_v);




}