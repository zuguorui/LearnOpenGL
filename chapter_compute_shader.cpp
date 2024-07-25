#include "chapter_compute_shader.h"
#include "ComputeProgram.h"
#include "GLWindow.h"
#include "default_gl_include.h"
#include "RenderProgram.h"
#include "default_stb_include.h"
#include "pixel_loader.h"
#include <_types/_uint8_t.h>
#include <cstring>
#include <sys/_types/_null.h>

using namespace std;

typedef void (APIENTRY *DEBUGPROC)(GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar *message,
            const void *userParam);
void GL_DEBUG_CALLBACK(GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar *message,
            const void *userParam) {

    cout << "OpenGL output: " << message << endl;
}

// 使用PBO来传递纹理数据。这里演示的是发送到GL纹理，也可以从帧缓冲中读取数据到本地内存。
void testPBO() {
    
    // begin: 加载纹理
    int width, height, numChannels;
    uint8_t *data = nullptr;

    data = stbi_load("./images/wall.jpg", &width, &height, &numChannels, 0);

    if (!data) {
        cout << "load image failed" << endl;
        return;
    }

    int totalSize = width * height * numChannels;

    cout << "width = " << width <<  ", height = " << height << ", numChannels = " << numChannels << endl;

    GLWindow glWindow = GLWindow::Builder().setTitle("Pixel Buffer Object").setSize(width, height).build();

    RenderProgram program("./shaders/textures.vs", "./shaders/textures.frag");

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

    
    GLuint pixelBuffer;
    glGenBuffers(1, &pixelBuffer);
    // 绑定到UNPACK buffer。GL -> client是PACK，client -> GL是UNPACK。
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixelBuffer);
    // 向buffer加载数据，调用此方法，GL会自动分配所需大小的内存空间。
    glBufferData(GL_PIXEL_UNPACK_BUFFER, totalSize, nullptr, GL_STREAM_DRAW);
    // glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // 前面已经绑定了pixelBuffer。这里data传NULL，则代表该纹理绑定前面绑定的GL_PIXEL_UNPACK_BUFFER
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glGenerateMipmap(GL_TEXTURE_2D);

    program.use();

    while (!glfwWindowShouldClose(glWindow.window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);

        // 按下空格键，加载图片
        if (glfwGetKey(glWindow.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            cout << "space" << endl;
            if (data != nullptr) {
                cout << "upload" << endl;
                // 获取pxielBuffer的指针，可以直接向里面写数据。
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixelBuffer);
                void *ref = glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, totalSize, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
                memcpy(ref, data, totalSize);
                glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, NULL);
                stbi_image_free(data);
                data = nullptr;
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
            }
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
        
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &pixelBuffer);
    glDeleteTextures(1, &tex);


}

// 分离yuv或rgb数据，并绘制出来。这里以NV21为例
// mac仅支持到OpenGL 4.1，换手机进行测试
void testComputeShader() {

    int width, height;
    int bitDepth;
    uint8_t *data = nullptr;
    
    if (!load_rgb565_data(&width, &height, &data)) {
        cout << "load picture failed" << endl;
        return;
    }

    int elementSize = sizeof(int16_t);
    int pixelCount = width * height;
    cout << "width = " << width << ", height = " << height << endl;

    GLWindow glWindow = GLWindow::Builder().setTitle("compute shader").setSize(width, height).build();

    // glDebugMessageCallback(GL_DEBUG_CALLBACK, nullptr);

    // 申请原始数据存储空间
    GLuint rawBuffer;
    glGenBuffers(1, &rawBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, rawBuffer);
    glBufferData(GL_ARRAY_BUFFER, elementSize * pixelCount, data, GL_DYNAMIC_READ);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 申请三通道的buffer，用来存储分离后的通道数据。分离之后的数据都是uint8_t的
    GLuint ryBuffer, guBuffer, bvBuffer;
    GLuint compBuffers[3] = {ryBuffer, guBuffer, bvBuffer};
    glGenBuffers(3, compBuffers);
    
    glBindBuffer(GL_ARRAY_BUFFER, ryBuffer);
    glBufferData(GL_ARRAY_BUFFER, elementSize * pixelCount, nullptr, GL_DYNAMIC_READ);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, guBuffer);
    glBufferData(GL_ARRAY_BUFFER, elementSize * pixelCount, nullptr, GL_DYNAMIC_READ);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, bvBuffer);
    glBufferData(GL_ARRAY_BUFFER, elementSize * pixelCount, nullptr, GL_DYNAMIC_READ);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 创建绑定到上面buffer的texture
    GLuint rawTex, ryTex, guTex, bvTex;

    glGenTextures(1, &rawTex);
    glBindTexture(GL_TEXTURE_BUFFER, rawTex);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_R16UI, rawBuffer);
    glBindTexture(GL_TEXTURE_BUFFER, 0);

    glGenTextures(1, &ryTex);
    glBindTexture(GL_TEXTURE_BUFFER, ryTex);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_R8UI, ryBuffer);
    glBindTexture(GL_TEXTURE_BUFFER, 0);

    glGenTextures(1, &guTex);
    glBindTexture(GL_TEXTURE_BUFFER, guTex);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_R8UI, guBuffer);
    glBindTexture(GL_TEXTURE_BUFFER, 0);

    glGenTextures(1, &bvTex);
    glBindTexture(GL_TEXTURE_BUFFER, bvTex);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_R8UI, bvBuffer);
    glBindTexture(GL_TEXTURE_BUFFER, 0);

    ComputeProgram computeProgram("./shaders/split_rgb565.frag");
    if (!computeProgram.isReady()) {
        cout << "compute program not ready" << endl;
        return;
    }
    computeProgram.use();
    
    glBindImageTexture(0, rawTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R16UI);
    glBindImageTexture(1, ryTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R8UI);
    glBindImageTexture(1, guTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R8UI);
    glBindImageTexture(1, bvTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R8UI);

    computeProgram.setInt("width", width);
    computeProgram.setInt("height", height);

    glDispatchCompute(width / 16, height / 16, 1);

    GLsync sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    glFlush();

    glWaitSync(sync, GL_SYNC_GPU_COMMANDS_COMPLETE, GL_TIMEOUT_IGNORED);
    glDeleteSync(sync);

    glDeleteTextures(1, &rawTex);
    glDeleteTextures(1, &ryTex);
    glDeleteTextures(1, &guTex);
    glDeleteTextures(1, &bvTex);

    // 下个阶段，准备绘制

    // 分别构造rgb三个通道的texture
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, ryBuffer);
    glGenTextures(1, &ryTex);
    glBindTexture(GL_TEXTURE_2D, ryTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, width, height, 0, GL_R8UI, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, guBuffer);
    glGenTextures(1, &guTex);
    glBindTexture(GL_TEXTURE_2D, guTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, width, height, 0, GL_R8UI, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, bvBuffer);
    glGenTextures(1, &bvTex);
    glBindTexture(GL_TEXTURE_2D, bvTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, width, height, 0, GL_R8UI, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);


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

    RenderProgram renderProgram("./shaders/textures.vs", "./shaders/render_rgb.frag");

    renderProgram.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ryTex);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, guTex);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, bvTex);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(glWindow.window);

    while (!glfwWindowShouldClose(glWindow.window)) {
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glDeleteTextures(1, &rawTex);
    glDeleteTextures(1, &ryTex);
    glDeleteTextures(1, &guTex);
    glDeleteTextures(1, &bvTex);

    glDeleteBuffers(1, &rawBuffer);
    glDeleteBuffers(1, &ryBuffer);
    glDeleteBuffers(1, &guBuffer);
    glDeleteBuffers(1, &bvBuffer);

}