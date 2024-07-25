#include "GLWindow.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;


static void defaultFrameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void defaultKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

GLWindow::Builder::~Builder() {
    if (this->title) {
        free(this->title);
        this->title = nullptr;
    }
}

GLWindow::Builder& GLWindow::Builder::setTitle(const char *title) {
    if (this->title) {
        free(this->title);
    }
    this->title = (char *)malloc(strlen(title));
    strcpy(this->title, title);
    return *this;
}

GLWindow::Builder& GLWindow::Builder::setSize(int width, int height) {
    this->width = width;
    this->height = height;
    return *this;
}

GLWindow::Builder& GLWindow::Builder::setKeyCallback(GLFWkeyfun callback) {
    this->keyCallback = callback;
    return *this;
}

GLWindow::Builder& GLWindow::Builder::setCursorPosCallback(GLFWcursorposfun callback) {
    this->cursorPosCallback = callback;
    return *this;
}

GLWindow::Builder& GLWindow::Builder::setFrameSizeCallback(GLFWframebuffersizefun callback) {
    this->frameSizeCallback = callback;
    return *this;
}

GLWindow GLWindow::Builder::build() {
    return GLWindow(
        this->title == nullptr ? "No title" : this->title,
        this->width, this->height,
        this->keyCallback == nullptr ? defaultKeyCallback : this->keyCallback,
        this->cursorPosCallback,
        this->frameSizeCallback == nullptr ? defaultFrameBufferSizeCallback : this->frameSizeCallback
    );
}

GLWindow::GLWindow(GLWindow&& src) {
    printf("GLWindow: move constructor\n");
    this->window = src.window;
    src.window = nullptr;
    
}

GLWindow::GLWindow(const char *title, int width, int height, GLFWkeyfun keyCallback, GLFWcursorposfun cursorPosCallback,     
                    GLFWframebuffersizefun frameSizeCallback) {
    printf("GLWindow: constructor\n");
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        printf("create window failed\n");
        glfwTerminate();
        return;
    }
    int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    cout << "glfw version: " << major << "." << minor << endl;
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameSizeCallback);

    if (keyCallback) {
        glfwSetKeyCallback(window, keyCallback);
    }

    if (cursorPosCallback) {
        glfwSetCursorPosCallback(window, cursorPosCallback);
    }
    

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("glad load failed\n");
        glfwTerminate();
        glfwDestroyWindow(window);
        window = nullptr;
        return;
    }
}

bool GLWindow::success() {
    return window != nullptr;
}

GLWindow::~GLWindow() {
    printf("GLWindow: destructor\n");
    glfwTerminate();
}