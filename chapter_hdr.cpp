
#include "chapter_hdr.h"
#include "default_gl_include.h"
#include "RenderProgram.h"
#include "GLWindow.h"

#define SRC_WIDTH 800
#define SRC_HEIGHT 400

using namespace std;
using namespace Imf;
using namespace Imath;

// 从exr文件中读取数据并转换为RGB数据，像素格式为float，直接存到文件中。
void export_hdr_raw() {
    const char *path = "./assets/shanghai_bund_4k.exr";
    Array2D<float> rPixels, gPixels, bPixels;
    int width = -1, height = -1;

    read_hdr_file(path, width, height, rPixels, gPixels, bPixels);

    if (width <= 0 || height <= 0)
    {
        cout << "failed to load hdr file" << endl;
        return;
    }


    FILE *f = fopen("./assets/hdr_export.rgb", "wb");
}

void display_hdr_image()
{

    GLWindow glWindow = GLWindow::Builder().setTitle("hdr").setSize(SRC_WIDTH, SRC_HEIGHT).build();

    const char *path = "./assets/Desk.exr";

    Array2D<float> rPixels, gPixels, bPixels;
    int width = -1, height = -1;

    read_hdr_file(path, width, height, rPixels, gPixels, bPixels);

    if (width <= 0 || height <= 0)
    {
        cout << "failed to load hdr file" << endl;
        return;
    }

    int start = 0, end = height - 1;
    float tmp;
    while (start < end)
    {
        for (int i = 0; i < width; i++)
        {
            tmp = rPixels[start][i];
            rPixels[start][i] = rPixels[end][i];
            rPixels[end][i] = tmp;

            tmp = gPixels[start][i];
            gPixels[start][i] = gPixels[end][i];
            gPixels[end][i] = tmp;

            tmp = bPixels[start][i];
            bPixels[start][i] = bPixels[end][i];
            bPixels[end][i] = tmp;
        }
        start++;
        end--;
    }

    // for (int i = 0; i < height; i++) {
    //     for (int j = 0; j < width; j++) {
    //         if (rPixels[i][j] > 1) {
    //             printf("r pixel at [%d][%d] is %f\n", i, j, rPixels[i][j]);
    //         }
    //         if (gPixels[i][j] > 1) {
    //             printf("g pixel at [%d][%d] is %f\n", i, j, gPixels[i][j]);
    //         }
    //         if (bPixels[i][j] > 1) {
    //             printf("b pixel at [%d][%d] is %f\n", i, j, bPixels[i][j]);
    //         }
    //     }
    // }

    const GLuint texFormat = GL_R32F;
    const GLuint dataFormat = GL_RED;

    GLuint rTex;
    glGenTextures(1, &rTex);
    glBindTexture(GL_TEXTURE_2D, rTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, texFormat, width, height, 0, dataFormat, GL_FLOAT, &rPixels[0][0]);
    glGenerateMipmap(GL_TEXTURE_2D);

    GLuint gTex;
    glGenTextures(1, &gTex);
    glBindTexture(GL_TEXTURE_2D, gTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, texFormat, width, height, 0, dataFormat, GL_FLOAT, &gPixels[0][0]);
    glGenerateMipmap(GL_TEXTURE_2D);

    GLuint bTex;
    glGenTextures(1, &bTex);
    glBindTexture(GL_TEXTURE_2D, bTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, texFormat, width, height, 0, dataFormat, GL_FLOAT, &bPixels[0][0]);
    glGenerateMipmap(GL_TEXTURE_2D);

    float quadVertices[] = {// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                            // positions   // texCoords
                            -1.0f, 1.0f, 0.0f, 1.0f,
                            -1.0f, -1.0f, 0.0f, 0.0f,
                            1.0f, -1.0f, 1.0f, 0.0f,

                            -1.0f, 1.0f, 0.0f, 1.0f,
                            1.0f, -1.0f, 1.0f, 0.0f,
                            1.0f, 1.0f, 1.0f, 1.0f};

    GLuint quadVBO;
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), (const void *)quadVertices, GL_STATIC_DRAW);

    GLuint quadVAO;
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    RenderProgram mShader("./shaders/hdr.vs", "./shaders/hdr.frag");

    mShader.use();
    mShader.setInt("tex_r", 0);
    mShader.setInt("tex_g", 1);
    mShader.setInt("tex_b", 2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rTex);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gTex);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, bTex);

    while (!glfwWindowShouldClose(glWindow.window))
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(quadVAO);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &quadVAO);
}