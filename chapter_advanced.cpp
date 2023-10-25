#include "chapter_advanced.h"

#include "default_gl_include.h"
#include "Shader.h"
#include "GLWindow.h"
#include "FPSCamera.h"
#include "data.h"
#include "util.h"

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

void testFrameBuffer() {
    GLWindow glWindow = GLWindow::Builder().setSize(SRC_WIDTH, SRC_HEIGHT).build();
    glfwSetCursorPosCallback(glWindow.window, mouse_callback);
    glfwSetInputMode(glWindow.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    float planeVertices[] = {
        // positions          // texture Coords 
         5.0f, -0.5f,  5.0f,  1.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 1.0f,

         5.0f, -0.5f,  5.0f,  1.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 1.0f,
         5.0f, -0.5f, -5.0f,  1.0f, 1.0f
    };
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    // 创建箱子的顶点数据
    GLuint cubeVBO;
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), (const void *)cubeVertices, GL_STATIC_DRAW);

    GLuint cubeVAO;
    glGenVertexArrays(1, &cubeVAO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // 创建地板的顶点数据
    GLuint planeVBO;
    glGenBuffers(1, &planeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), (const void *)planeVertices, GL_STATIC_DRAW);

    GLuint planeVAO;
    glGenVertexArrays(1, &planeVAO);

    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // 创建frameBuffer的顶点数据，它是一个二维的矩形的绘制区域
    GLuint quadVBO;
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), (const void *)quadVertices, GL_STATIC_DRAW);

    GLuint quadVAO;
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);


    // 创建箱子和地板的纹理
    GLuint cubeTexture = loadTexture("./images/container.jpg");

    GLuint floorTexture = loadTexture("./images/metal.png");

    Shader shader("./shaders/frame_buffer.vs", "./shaders/frame_buffer.frag");
    Shader screenShader("./shaders/frame_buffer_screen.vs", "./shaders/frame_buffer_screen.frag");

    shader.use();
    shader.setInt("texture1", 0);
    

    screenShader.use();
    screenShader.setInt("screenTexture", 0);

    // 创建frameBuffer
    GLuint frameBuffer;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    // 为frameBuffer创建一个颜色纹理，对frameBuffer的渲染都会存放到这个纹理中
    // 注意，这里长宽*2的原因是因为在mac的高分屏下，四个像素合并渲染一个像素，否则的话会导致画面异常
    GLuint textureColorBuffer;
    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SRC_WIDTH * 2, SRC_HEIGHT * 2, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 将这个纹理附加到帧缓冲上
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

    // 建立具有深度测试的渲染缓冲对象，注意其内部格式是GL_DEPTH24_STENCIL8
    // 普通纹理和渲染缓冲对象的区别是渲染缓冲对象不会转换格式，而是直接使用OpenGL的内部颜色格式，因此它更快，但不支持读取像素。
    // 所以这里颜色附件使用普通纹理，而深度附件使用渲染缓冲对象
    GLuint RBO;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SRC_WIDTH * 2, SRC_HEIGHT * 2);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        cout << "Error: Framebuffer is not complete" << endl;
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Direction direction = Direction::NONE;
    double lastTime = glfwGetTime();


    while (!glfwWindowShouldClose(glWindow.window))
    {
        
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

        // 绑定FrameBuffer，这会导致后面的绘制都在FrameBuffer上而不是屏幕上。
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        // 启动深度测试
        glEnable(GL_DEPTH_TEST);

        // 清除FrameBuffer的数据
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 绘制箱子和地板
        shader.use();
        shader.setMat4("projection", camera.projection);
        shader.setMat4("view", camera.view);

        glm::mat4 model(1.0f);

        // 绘制箱子
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::translate(model,  glm::vec3(2.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        // 绘制地板
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // 解绑FrameBuffer，到这一步，之前的箱子和地板都绘制到了帧缓冲里
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 注意这里，由于之后的帧缓冲只是一个二维图像，我们要做的仅仅是将这幅图像渲染到
        // 屏幕上，所以这里禁用的是屏幕的深度测试
        glDisable(GL_DEPTH_TEST);

        // 这里清除的是屏幕的内容
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // 之前绘制的内容已经在帧缓冲的颜色附件中（也就是textureColorBuffer），之后只要把这个纹理像往常一样绘制到屏幕上即可。
        screenShader.use();
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteVertexArrays(1, &quadVAO);

    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteBuffers(1, &quadVBO);

    glDeleteFramebuffers(1, &frameBuffer);
    glDeleteRenderbuffers(1, &RBO);

}

void testFaceCulling() {

    // 箱子的面顶点和地板顶点都按照顺时针环绕排列
    float cubeVertices[] = {
        // positions          // texture Coords
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right    
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right              
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left                
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right        
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left        
        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left       
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        // Right face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right      
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right          
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
        // Bottom face          
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left        
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right                 
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // bottom-left  
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f  // top-left              
    };

     float planeVertices[] = {
        // positions          // texture Coords 
         5.0f, -0.5f,  5.0f,  1.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 1.0f,
        
         5.0f, -0.5f,  5.0f,  1.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 1.0f,
         5.0f, -0.5f, -5.0f,  1.0f, 1.0f,
    };

    GLWindow glWindow = GLWindow::Builder().setSize(SRC_WIDTH, SRC_HEIGHT).build();
    glfwSetCursorPosCallback(glWindow.window, mouse_callback);
    glfwSetInputMode(glWindow.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // 启动深度测试
    glEnable(GL_DEPTH_TEST);
    
    // 启动面剔除
    glEnable(GL_CULL_FACE); 
    // 剔除背面
    glCullFace(GL_BACK); 
    // 顺时针环绕定义为正面，默认为逆时针是GL_CCW。
    glFrontFace(GL_CW);

    // 创建箱子的顶点数据
    GLuint cubeVBO;
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), (const void *)cubeVertices, GL_STATIC_DRAW);

    GLuint cubeVAO;
    glGenVertexArrays(1, &cubeVAO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // 创建地板的顶点数据
    GLuint planeVBO;
    glGenBuffers(1, &planeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), (const void *)planeVertices, GL_STATIC_DRAW);

    GLuint planeVAO;
    glGenVertexArrays(1, &planeVAO);

    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // 创建箱子和地板的纹理
    GLuint cubeTexture = loadTexture("./images/container.jpg");
    GLuint floorTexture = loadTexture("./images/metal.png");

    Shader shader("./shaders/frame_buffer.vs", "./shaders/frame_buffer.frag");
    shader.use();
    shader.setInt("texture1", 0);

    Direction direction = Direction::NONE;
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(glWindow.window)) {

        // 这里清除的是屏幕的内容
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


        // 绘制箱子和地板
        shader.use();
        shader.setMat4("projection", camera.projection);
        shader.setMat4("view", camera.view);
        glm::mat4 model(1.0f);

        // 绘制箱子
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::translate(model,  glm::vec3(2.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        // 绘制地板
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);



        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);

    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);

}