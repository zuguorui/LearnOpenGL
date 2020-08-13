#ifndef _TRIANGLE_DRAWER_H_
#define _TRIANGLE_DRAWER_H_

#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;


GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
};

const char *vertexShaderSource = 
    "#version 300 core\n"
    "layout (location = 0) in vec3 position;\n"
    "void main{\n"
    "   gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "}";


class TriangleDrawer
{
public:

    bool init();
    void drawTriangle();

private:
    GLuint vertexShader;
};




#endif
