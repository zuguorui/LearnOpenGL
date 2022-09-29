#include "GLWindow.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void defaultFrameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void defaultKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

GLWindow::Builder& GLWindow::Builder::setTitle(const char *title) {
    if (this->title) {
        free[] this->title;
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

GLWindow::Builder& GLWindow::Builder::setFrameSizeCallback(GLFWframebuffersizefun callback) {
    this->frameSizeCallback = callback;
    return *this;
}

GLWindow::Builder& GLWindow::Builder::build() {
    return new GLWindow(
        this->title == nullptr ? " " : this->title,
        this->width, this->height,
        this->keyCallback == nullptr ? defaultKeyCallback : this->keyCallback,
        this->frameSizeCallback == nullptr ? defaultFrameBufferSizeCallback : this->frameSizeCallback
    );
}

GLWindow::GLWindow(const char *title, int width, int height, GLFWkeyfun keyCallback, GLFWframebuffersizefun frameSizeCallback) {
    
}