#include "chapter_light.h"
#include "GLWindow.h"
#include "default_gl_include.h"
#include "FPSCamera.h"
#include "Shader.h"

#include <iostream>
#include <stdlib.h>

#define SRC_WIDTH 800
#define SRC_HEIGHT 400

using namespace std;

// 单个立方体的顶点坐标。六个面，每个面两个三角形。
// 这里没有用到EBO。
static float cube_vertices[] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };

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

static glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void testLight()
{
    GLWindow glWindow = GLWindow::Builder().setSize(SRC_WIDTH, SRC_HEIGHT).build();
    glfwSetCursorPosCallback(glWindow.window, mouse_callback);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(glWindow.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLuint cube_VAO;
    GLuint VBO;
    glGenVertexArrays(1, &cube_VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), (const void *)cube_vertices, GL_STATIC_DRAW);

    glBindVertexArray(cube_VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);


    GLuint light_VAO;
    glGenVertexArrays(1, &light_VAO);
    glBindVertexArray(light_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);


    Shader cubeShader("./shaders/pure_color_cube.vs", "./shaders/pure_color_cube.frag");
    Shader lightShader("./shaders/pure_color_cube.vs", "./shaders/pure_color_cube.frag");
    cubeShader.use();
    lightShader.use();

    

    double lastTime = glfwGetTime();
    Direction direction = Direction::NONE;

    while (!glfwWindowShouldClose(glWindow.window))
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
    
        direction = Direction::NONE;
        if (glfwGetKey(glWindow.window, GLFW_KEY_W) == GLFW_PRESS)
        {
            direction = Direction::FORWARD;
        }
        if (glfwGetKey(glWindow.window, GLFW_KEY_S) == GLFW_PRESS)
        {
            direction = Direction::BACKWARD;
        }
        if (glfwGetKey(glWindow.window, GLFW_KEY_A) == GLFW_PRESS)
        {
            direction = Direction::LEFT;
        }
        if (glfwGetKey(glWindow.window, GLFW_KEY_D) == GLFW_PRESS)
        {
            direction = Direction::RIGHT;
        }

        camera.updatePos(direction, (float)(glfwGetTime() - lastTime));
        lastTime = glfwGetTime();

        cubeShader.use();
        glBindVertexArray(cube_VAO);
        cubeShader.setMat4("projection", camera.projection);
        cubeShader.setMat4("view", camera.view);
        glm::mat4 cubeModel(1.0f);
        cubeShader.setMat4("model", cubeModel);
        cubeShader.setVec3("cubeColor", 1.0f, 0.5f, 0.31f);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        lightShader.use();
        glBindVertexArray(light_VAO);
        glm::mat4 lightModel(1.0f);
        lightShader.setMat4("projection", camera.projection);
        lightShader.setMat4("view", camera.view);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        
        lightShader.setMat4("model", lightModel);
        lightShader.setVec3("cubeColor", 1.0f, 1.0f, 1.0f);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &cube_VAO);
    glDeleteVertexArrays(1, &light_VAO);
}