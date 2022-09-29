//
//  WindowUtil.cpp
//  LearnOpenGL-xcode
//
//  Created by zhiyun on 2020/8/17.
//  Copyright © 2020 zu. All rights reserved.
//

#include "WindowUtil.h"

using namespace std;

void frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

bool initWindow(GLFWwindow **window)
{
    // MUST init a window and set it current first so you have a OpenGL context.

    if(!glfwInit())
    {
        cout << "Error glfwInit" << endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // need set this when use mac OS and xcode

    *window = glfwCreateWindow(640, 480, "Hello world", NULL, NULL);

    if(!(*window))
    {
        glfwTerminate();
        return false;
    }


    glfwMakeContextCurrent(*window);

    glfwSetKeyCallback(*window, key_callback);
    glfwSetFramebufferSizeCallback(*window, frameBufferSizeCallback);
    
    return true;
}

void destroyWindow()
{
    glfwTerminate();
}
