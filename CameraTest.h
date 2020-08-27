#pragma once

#include <iostream>
#include <stdlib.h>
#include "stb_include.h"
#include "WindowUtil.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;

class CameraTest
{
public:
    void draw();
    void freeVision();

private:

    static void mouse_callback(GLFWwindow *window, double xPos, double yPos);
    static void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);
    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

    void processMouse(double xPos, double yPos);
    void processScroll(double xOffset, double yOffset);
    void processKey(int key, int scancode, int action, int mode);

    glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);

    bool firstMouse = false;
    float yaw = -90.0f;
    float pitch = -0.0f;
    float lastX = 800.0f / 2;
    float lastY = 600.0f / 2;

    float fov = 45.0f;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    GLFWwindow *window = NULL;
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
};