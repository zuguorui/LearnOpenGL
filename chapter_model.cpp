#include "chapter_model.h"
#include "GLWindow.h"
#include "default_gl_include.h"
#include "util.h"
#include "FPSCamera.h"

#include <iostream>
#include <stdlib.h>

using namespace std;

#define SRC_WIDTH 800
#define SRC_HEIGHT 400

static bool firstMouse = true;
static float lastX = SRC_WIDTH / 2;
static float lastY = SRC_HEIGHT / 2;

static FPSCamera camera = FPSCamera::Builder()
                       .setFov(45.0)
                       .setFovRation(SRC_WIDTH * 1.0f / SRC_HEIGHT)
                       .setPitch(0)
                       .setYaw(-90.0)
                       .setPos(glm::vec3(0, 10, 10))
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

void testLoadModel() {
    GLWindow glWindow = GLWindow::Builder().setSize(SRC_WIDTH, SRC_HEIGHT).build();
    glfwSetCursorPosCallback(glWindow.window, mouse_callback);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(glWindow.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Model model = Model("./assets/nanosuit/nanosuit.obj");
    Shader shader = Shader("./shaders/load_model.vs", "./shaders/load_model.frag");
    shader.use();
    shader.setMat4("model", glm::mat4(1.0f));

    Direction direction = Direction::NONE;
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(glWindow.window)) {

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
    
        direction = Direction::NONE;
        if (glfwGetKey(glWindow.window, GLFW_KEY_W) == GLFW_PRESS) {
            direction = Direction::FORWARD;
        }
        if (glfwGetKey(glWindow.window, GLFW_KEY_S) == GLFW_PRESS) {
            direction = Direction::BACKWARD;
        }
        if (glfwGetKey(glWindow.window, GLFW_KEY_A) == GLFW_PRESS) {
            direction = Direction::LEFT;
        }
        if (glfwGetKey(glWindow.window, GLFW_KEY_D) == GLFW_PRESS) {
            direction = Direction::RIGHT;
        }
        camera.updatePos(direction, (float)(glfwGetTime() - lastTime));
        lastTime = glfwGetTime();

        shader.use();
        shader.setMat4("projection", camera.projection);
        shader.setMat4("view", camera.view);

        model.draw(shader);

        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();

    }


}