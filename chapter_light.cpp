#include "chapter_light.h"
#include "GLWindow.h"
#include "default_gl_include.h"
#include "FPSCamera.h"
#include "Shader.h"
#include "default_stb_include.h"
#include "util.h"
#include "data.h"

#include <iostream>
#include <stdlib.h>

#define SRC_WIDTH 800
#define SRC_HEIGHT 400

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

static glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void testLight()
{
    GLWindow glWindow = GLWindow::Builder().setSize(SRC_WIDTH, SRC_HEIGHT).build();
    glfwSetCursorPosCallback(glWindow.window, mouse_callback);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(glWindow.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLuint pos_VBO;
    glGenBuffers(1, &pos_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), (const void *)cube_vertices, GL_STATIC_DRAW);

    GLuint cube_VAO, normal_VBO;
    glGenVertexArrays(1, &cube_VAO);
    glGenBuffers(1, &normal_VBO);

    glBindVertexArray(cube_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, normal_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_normals), (const void *)cube_normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    


    GLuint light_VAO;
    glGenVertexArrays(1, &light_VAO);
    glBindVertexArray(light_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);


    Shader cubeShader("./shaders/3d_light_cube.vs", "./shaders/3d_light_cube.frag");
    Shader lightShader("./shaders/3d_color_cube.vs", "./shaders/3d_color_cube.frag");
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
        cubeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        cubeShader.setVec3("lightPos", lightPos);
        cubeShader.setVec3("viewPos", camera.pos);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        lightShader.use();
        glBindVertexArray(light_VAO);
        lightShader.setMat4("projection", camera.projection);
        lightShader.setMat4("view", camera.view);
        glm::mat4 lightModel(1.0f);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        lightShader.setMat4("model", lightModel);
        lightShader.setVec3("cubeColor", 1.0f, 1.0f, 1.0f);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &pos_VBO);
    glDeleteBuffers(1, &normal_VBO);
    glDeleteVertexArrays(1, &cube_VAO);
    glDeleteVertexArrays(1, &light_VAO);
}

void testMaterial() {
    GLWindow glWindow = GLWindow::Builder().setSize(SRC_WIDTH, SRC_HEIGHT).build();
    glfwSetCursorPosCallback(glWindow.window, mouse_callback);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(glWindow.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLuint pos_VBO;
    glGenBuffers(1, &pos_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), (const void *)cube_vertices, GL_STATIC_DRAW);

    GLuint cube_VAO, normal_VBO;
    glGenVertexArrays(1, &cube_VAO);
    glGenBuffers(1, &normal_VBO);

    glBindVertexArray(cube_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, normal_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_normals), (const void *)cube_normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    


    GLuint light_VAO;
    glGenVertexArrays(1, &light_VAO);
    glBindVertexArray(light_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);


    Shader cubeShader("./shaders/material_cube.vs", "./shaders/material_cube.frag");
    Shader lightShader("./shaders/3d_color_cube.vs", "./shaders/3d_color_cube.frag");
    cubeShader.use();
    lightShader.use();

    

    double lastTime = glfwGetTime();
    Direction direction = Direction::NONE;

    glm::vec3 lightColor(0.0f);

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

        lightColor.r = sin(glfwGetTime() * 2.0f);
        lightColor.g = sin(glfwGetTime() * 0.7f);
        lightColor.b = sin(glfwGetTime() * 1.3f);

        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

        cubeShader.use();
        glBindVertexArray(cube_VAO);
        cubeShader.setMat4("projection", camera.projection);
        cubeShader.setMat4("view", camera.view);
        glm::mat4 cubeModel(1.0f);
        cubeShader.setMat4("model", cubeModel);
        cubeShader.setVec3("cubeColor", 1.0f, 0.5f, 0.31f);
        cubeShader.setVec3("viewPos", camera.pos);
        cubeShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        cubeShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        cubeShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        cubeShader.setFloat("material.shininess", 32.0f);
        cubeShader.setVec3("light.ambient", ambientColor);
        cubeShader.setVec3("light.diffuse", diffuseColor);
        cubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setVec3("light.position", lightPos);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        lightShader.use();
        glBindVertexArray(light_VAO);
        lightShader.setMat4("projection", camera.projection);
        lightShader.setMat4("view", camera.view);
        glm::mat4 lightModel(1.0f);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        lightShader.setMat4("model", lightModel);
        lightShader.setVec3("cubeColor", lightColor);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &pos_VBO);
    glDeleteBuffers(1, &normal_VBO);
    glDeleteVertexArrays(1, &cube_VAO);
    glDeleteVertexArrays(1, &light_VAO);
}



void testLightMap() {
    GLWindow glWindow = GLWindow::Builder().setSize(SRC_WIDTH, SRC_HEIGHT).build();
    glfwSetCursorPosCallback(glWindow.window, mouse_callback);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(glWindow.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLuint posVBO;
    glGenBuffers(1, &posVBO);
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), (const void *)cube_vertices, GL_STATIC_DRAW);

    GLuint cube_VAO;
    GLuint normal_VBO, texCoord_VBO;
    glGenVertexArrays(1, &cube_VAO);

    glGenBuffers(1, &normal_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normal_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_normals), (const void *)cube_normals, GL_STATIC_DRAW);

    glGenBuffers(1, &texCoord_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, texCoord_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_tex_coords), (const void *)cube_tex_coords, GL_STATIC_DRAW);

    glBindVertexArray(cube_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, normal_VBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, texCoord_VBO);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    


    GLuint light_VAO;
    glGenVertexArrays(1, &light_VAO);
    glBindVertexArray(light_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    Shader cubeShader("./shaders/light_map.vs", "./shaders/light_map.frag");
    Shader lightShader("./shaders/3d_color_cube.vs", "./shaders/3d_color_cube.frag");
    cubeShader.use();
    lightShader.use();

    GLuint diffuseTexture = loadTexture("./images/container2.png");
    if (diffuseTexture == 0) {
        cout << "load texture failed" << endl;
        return;
    }

    GLuint specularTexture = loadTexture("./images/container2_specular.png");
    if (specularTexture == 0) {
        cout << "load texture failed" << endl;
        return;
    }



    Direction direction = Direction::NONE;
    double lastTime = glfwGetTime();
    glm::vec3 lightColor(1.0f);

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

        

        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

        cubeShader.use();
        glBindVertexArray(cube_VAO);
        cubeShader.setMat4("projection", camera.projection);
        cubeShader.setMat4("view", camera.view);
        glm::mat4 cubeModel(1.0f);
        cubeShader.setMat4("model", cubeModel);
        cubeShader.setVec3("viewPos", camera.pos);

        cubeShader.setFloat("material.shininess", 64.0f);
        
        cubeShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        cubeShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        cubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setVec3("light.position", lightPos);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularTexture);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        lightShader.use();
        glBindVertexArray(light_VAO);
        lightShader.setMat4("projection", camera.projection);
        lightShader.setMat4("view", camera.view);
        glm::mat4 lightModel(1.0f);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        lightShader.setMat4("model", lightModel);
        lightShader.setVec3("cubeColor", lightColor);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &posVBO);
    glDeleteBuffers(1, &normal_VBO);
    glDeleteBuffers(1, &texCoord_VBO);
    glDeleteVertexArrays(1, &cube_VAO);
    glDeleteVertexArrays(1, &light_VAO);
}


void testParallelLight() {
    GLWindow glWindow = GLWindow::Builder().setSize(SRC_WIDTH, SRC_HEIGHT).build();
    glfwSetCursorPosCallback(glWindow.window, mouse_callback);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(glWindow.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLuint posVBO;
    glGenBuffers(1, &posVBO);
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), (const void *)cube_vertices, GL_STATIC_DRAW);

    GLuint cube_VAO;
    GLuint normal_VBO, texCoord_VBO;
    glGenVertexArrays(1, &cube_VAO);

    glGenBuffers(1, &normal_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normal_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_normals), (const void *)cube_normals, GL_STATIC_DRAW);

    glGenBuffers(1, &texCoord_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, texCoord_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_tex_coords), (const void *)cube_tex_coords, GL_STATIC_DRAW);

    glBindVertexArray(cube_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, normal_VBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, texCoord_VBO);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);


    GLuint diffuseTexture = loadTexture("./images/container2.png");
    if (diffuseTexture == 0) {
        cout << "load texture failed" << endl;
        return;
    }

    GLuint specularTexture = loadTexture("./images/container2_specular.png");
    if (specularTexture == 0) {
        cout << "load texture failed" << endl;
        return;
    }

    Direction direction = Direction::NONE;
    double lastTime = glfwGetTime();

    Shader cubeShader("./shaders/parallel_light.vs", "./shaders/parallel_light.frag");
    cubeShader.use();

    cubeShader.setFloat("material.shininess", 64.0f);
    cubeShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    cubeShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    cubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    cubeShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
    
    cubeShader.setInt("material.diffuse", 0);
    cubeShader.setInt("material.specular", 1);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularTexture);

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


        
        glBindVertexArray(cube_VAO);
    
        

        cubeShader.setMat4("projection", camera.projection);
        cubeShader.setMat4("view", camera.view);
        cubeShader.setVec3("viewPos", camera.pos);

        for (int i = 0; i < NUM_CUBES; i++) {
            glm::mat4 cubeModel(1.0f);
            cubeModel = glm::translate(cubeModel, cube_pos[i]);
            cubeModel = glm::rotate(cubeModel, glm::radians(i * 17.0f), glm::vec3(1.0f, 0.3f, 0.5f));
            cubeShader.setMat4("model", cubeModel);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        glBindVertexArray(0);


        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &posVBO);
    glDeleteBuffers(1, &normal_VBO);
    glDeleteBuffers(1, &texCoord_VBO);
    glDeleteVertexArrays(1, &cube_VAO);


}

void testPointLight() {
    GLWindow glWindow = GLWindow::Builder().setSize(SRC_WIDTH, SRC_HEIGHT).build();
    glfwSetCursorPosCallback(glWindow.window, mouse_callback);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(glWindow.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLuint posVBO;
    glGenBuffers(1, &posVBO);
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), (const void *)cube_vertices, GL_STATIC_DRAW);

    GLuint cube_VAO;
    GLuint normal_VBO, texCoord_VBO;
    glGenVertexArrays(1, &cube_VAO);

    glGenBuffers(1, &normal_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normal_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_normals), (const void *)cube_normals, GL_STATIC_DRAW);

    glGenBuffers(1, &texCoord_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, texCoord_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_tex_coords), (const void *)cube_tex_coords, GL_STATIC_DRAW);

    glBindVertexArray(cube_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, normal_VBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, texCoord_VBO);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    GLuint light_VAO;
    glGenVertexArrays(1, &light_VAO);
    glBindVertexArray(light_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);


    GLuint diffuseTexture = loadTexture("./images/container2.png");
    if (diffuseTexture == 0) {
        cout << "load texture failed" << endl;
        return;
    }

    GLuint specularTexture = loadTexture("./images/container2_specular.png");
    if (specularTexture == 0) {
        cout << "load texture failed" << endl;
        return;
    }

    Direction direction = Direction::NONE;
    double lastTime = glfwGetTime();

    Shader cubeShader("./shaders/point_light.vs", "./shaders/point_light.frag");
    cubeShader.use();

    cubeShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    cubeShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    cubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    cubeShader.setVec3("light.position", lightPos);

    cubeShader.setFloat("light.constant", 1.0f);
    cubeShader.setFloat("light.linear", 0.09f);
    cubeShader.setFloat("light.quadratic", 0.032f);
    
    cubeShader.setFloat("material.shininess", 64.0f);
    
    cubeShader.setInt("material.diffuse", 0);
    cubeShader.setInt("material.specular", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularTexture);

    Shader lightShader("./shaders/3d_color_cube.vs", "./shaders/3d_color_cube.frag");
    lightShader.use();

    

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


        
        glBindVertexArray(cube_VAO);
        cubeShader.use();
        cubeShader.setMat4("projection", camera.projection);
        cubeShader.setMat4("view", camera.view);
        cubeShader.setVec3("viewPos", camera.pos);

        for (int i = 0; i < NUM_CUBES; i++) {
            glm::mat4 cubeModel(1.0f);
            cubeModel = glm::translate(cubeModel, cube_pos[i]);
            cubeModel = glm::rotate(cubeModel, glm::radians(i * 17.0f), glm::vec3(1.0f, 0.3f, 0.5f));
            cubeShader.setMat4("model", cubeModel);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        glBindVertexArray(0);

        
        glBindVertexArray(light_VAO);
        lightShader.use();
        lightShader.setMat4("projection", camera.projection);
        lightShader.setMat4("view", camera.view);
        glm::mat4 lightModel(1.0f);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        lightShader.setMat4("model", lightModel);
        lightShader.setVec3("cubeColor", 1.0f, 1.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);


        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &posVBO);
    glDeleteBuffers(1, &normal_VBO);
    glDeleteBuffers(1, &texCoord_VBO);
    glDeleteVertexArrays(1, &cube_VAO);

}

void testSpotLight() {
    GLWindow glWindow = GLWindow::Builder().setSize(SRC_WIDTH, SRC_HEIGHT).build();
    glfwSetCursorPosCallback(glWindow.window, mouse_callback);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(glWindow.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLuint posVBO;
    glGenBuffers(1, &posVBO);
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), (const void *)cube_vertices, GL_STATIC_DRAW);

    GLuint cube_VAO;
    GLuint normal_VBO, texCoord_VBO;
    glGenVertexArrays(1, &cube_VAO);

    glGenBuffers(1, &normal_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normal_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_normals), (const void *)cube_normals, GL_STATIC_DRAW);

    glGenBuffers(1, &texCoord_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, texCoord_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_tex_coords), (const void *)cube_tex_coords, GL_STATIC_DRAW);

    glBindVertexArray(cube_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, normal_VBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, texCoord_VBO);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);



    GLuint diffuseTexture = loadTexture("./images/container2.png");
    if (diffuseTexture == 0) {
        cout << "load texture failed" << endl;
        return;
    }

    GLuint specularTexture = loadTexture("./images/container2_specular.png");
    if (specularTexture == 0) {
        cout << "load texture failed" << endl;
        return;
    }

    Direction direction = Direction::NONE;
    double lastTime = glfwGetTime();

    Shader cubeShader("./shaders/spot_light.vs", "./shaders/spot_light.frag");
    cubeShader.use();

    cubeShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    cubeShader.setVec3("light.diffuse", 1.7f, 1.7f, 1.7f);
    cubeShader.setVec3("light.specular", 3.0f, 3.0f, 3.0f);
    cubeShader.setFloat("light.cutOff", glm::cos(glm::radians(10.0f)));
    cubeShader.setFloat("light.outerCutOff", glm::cos(glm::radians(40.0f)));

    cubeShader.setFloat("light.constant", 1.0f);
    cubeShader.setFloat("light.linear", 0.09f);
    cubeShader.setFloat("light.quadratic", 0.0032f);
    
    cubeShader.setFloat("material.shininess", 64.0f);
    
    cubeShader.setInt("material.diffuse", 0);
    cubeShader.setInt("material.specular", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularTexture);


    

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


        
        glBindVertexArray(cube_VAO);
        cubeShader.use();
        cubeShader.setMat4("projection", camera.projection);
        cubeShader.setMat4("view", camera.view);
        cubeShader.setVec3("viewPos", camera.pos);
        cubeShader.setVec3("light.position", camera.pos);
        cubeShader.setVec3("light.direction", camera.front);

        for (int i = 0; i < NUM_CUBES; i++) {
            glm::mat4 cubeModel(1.0f);
            cubeModel = glm::translate(cubeModel, cube_pos[i]);
            cubeModel = glm::rotate(cubeModel, glm::radians(i * 17.0f), glm::vec3(1.0f, 0.3f, 0.5f));
            cubeShader.setMat4("model", cubeModel);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        glBindVertexArray(0);

        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &posVBO);
    glDeleteBuffers(1, &normal_VBO);
    glDeleteBuffers(1, &texCoord_VBO);
    glDeleteVertexArrays(1, &cube_VAO);

}


