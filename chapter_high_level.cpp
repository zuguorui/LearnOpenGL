#include "chapter_high_level.h"

#include "default_gl_include.h"
#include "Shader.h"
#include "GLWindow.h"
#include "FPSCamera.h"
#include "data.h"
#include "util.h"

#define SRC_WIDTH 800
#define SRC_HEIGHT 600

using namespace std;

static bool firstMouse = true;
static float lastX = SRC_WIDTH / 2;
static float lastY = SRC_HEIGHT / 2;

static FPSCamera camera = FPSCamera::Builder()
                       .setFov(45.0)
                       .setFovRation(SRC_WIDTH * 1.0f / SRC_HEIGHT)
                       .setPitch(0)
                       .setYaw(-90.0)
                       .setPos(glm::vec3(0, 0, 5))
                       .setSensitivity(0.1f)
                       .setSpeed(1.0f)
                       .setWorldUp(glm::vec3(0, 1.0f, 0))
                       .build();

static void mouse_callback(GLFWwindow *window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.updateDirection(yOffset, xOffset);
}

void testFrameBuffer() {
    GLWindow glWindow = GLWindow::Builder().setSize(SRC_WIDTH, SRC_HEIGHT).setTitle("FrameBuffer test").build();
    glEnable(GL_DEPTH_TEST);
    glfwSetCursorPosCallback(glWindow.window, mouse_callback);
    glfwSetInputMode(glWindow.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    GLuint pos_VBO;
    glGenBuffers(1, &pos_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), (const void *)cube_vertices, GL_STATIC_DRAW);

    GLuint texCoord_VBO;
    glGenBuffers(1, &texCoord_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, texCoord_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_tex_coords), (const void *)cube_tex_coords, GL_STATIC_DRAW);

    GLuint cube_VAO;
    glGenVertexArrays(1, &cube_VAO);

    glBindVertexArray(cube_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, texCoord_VBO);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    GLuint tex = loadTexture("./images/container.jpg");

    
    

}