
#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

// https://learnopengl-cn.readthedocs.io/zh/latest

int main()
{
    GLFWwindow *window;

    if(!glfwInit())
    {
        cout << "Error glfwInit" << endl;
        return -1;
    }

    window = glfwCreateWindow(640, 480, "Hello world", NULL, NULL);

    if(!window)
    {
        glfwTerminate();
        return -1;
    }


    glfwMakeContextCurrent(window);

    while(!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}