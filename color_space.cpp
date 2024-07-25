#include "color_space.h"

#include "default_gl_include.h"
#include "RenderProgram.h"
#include "GLWindow.h"
#include "FPSCamera.h"

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

void draw_CIE_XYZ() {

    

    GLWindow glWindow = GLWindow::Builder().setTitle("CIE_XYZ").setSize(SRC_WIDTH, SRC_HEIGHT).build();
    glfwSetCursorPosCallback(glWindow.window, mouse_callback);
    glfwSetInputMode(glWindow.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
