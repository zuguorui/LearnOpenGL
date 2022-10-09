
#include "chapter_hello_triangle.h"
#include "util.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

/**
 * 顶点缓冲对象(VBO): Vertex Buffer Object
 * 顶点数组对象(VAO): Vertex Array Object
 * 
 */

void drawTriangle() {
    
    GLWindow glWindow = GLWindow::Builder().setTitle("Hello triangle").build();

    GLFWwindow *window = glWindow.window;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // begin: 设置顶点着色器
    string vertexShaderCode = readShaderProgram("./shaders/hello_triangle.vs");
    if (vertexShaderCode.size() == 0) {
        printf("read vertex shader code failed\n");
        return;
    }
    const char *vertexShaderCodeCStr = vertexShaderCode.c_str();
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCodeCStr, nullptr);
    glCompileShader(vertexShader);
    

    if (!checkShaderCompileStatus(vertexShader)) {
        return;
    }
    // end: 设置顶点着色器

    // begin: 设置片段着色器
    
    string fragmentShaderCode = readShaderProgram("./shaders/hello_triangle.frag");
    if (fragmentShaderCode.size() == 0) {
        printf("read fragment shader program failed\n");
        return;
    }
    const char* fragmentShaderCodeCStr = fragmentShaderCode.c_str();
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCodeCStr, nullptr);
    glCompileShader(fragmentShader);

    if (!checkShaderCompileStatus(fragmentShader)) {
        return;
    }
    // end: 设置片段着色器

    // begin: 设置着色器程序
    GLuint shaderProgram = glCreateProgram();
    // 将shader链接到program里
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    if (!checkProgramLinkStatus(shaderProgram)) {
        return;
    }

    // 链接之后，shader就可以删除了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // end: 设置着色器程序

    


    float vertices[] = {
            -0.5f, -0.5f,  0.0f,
             0.5f, -0.5f,  0.0f,
             0.0f,  0.5f,  0.0f
        };

    // 创建一个顶点缓冲对象VBO和一个顶点数组对象VAO
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 先绑定VAO，然后绑定并设置VBO，最后设置顶点属性。
    glBindVertexArray(VAO);

    // 将其绑定到GL_ARRAY_BUFFER上，之后对GL_ARRAY_BUFFER的任何操作都会
    // 配置当前绑定的VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 将定点数据传入GL_ARRAY_BUFFER，由于绑定了VBO，实际上也是缓存进了VBO中。
    // glBufferData有三个参数：
    // 1. buffer类型
    // 2. 缓存数据大小，单位是byte
    // 3. 数据指针
    // 4. 绘制类型。GL_STATIC_DRAW：数据几乎不变。GL_DYNAMIC_DRAW：数据会变很多。
    // GL_DRAW_STREAM：每次绘制数据都会变。
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 参数说明：
    // 1. index: 对应顶点着色器中属性的index，在hello_triangle.vs中，aPos的(location = 0)
    // 2. size: 顶点属性大小，是vec3，所以是3
    // 3. type: 数据类型，vec*是float的
    // 4. normalized: 是否标准化，如果是true，那么所有数据都会被映射到[-1, 1]之间
    // 5. stride: 步长，指两个坐标之间的间隔指。这里vertices是一个float数组，前一个坐标头与下一个坐标头相距3 * float。
    // 6. pointer: 位置数据在缓冲中起始位置的偏移量(Offset)，由于位置数据在数组的开头，所以这里是0。

    /**
     * 每个顶点属性从一个VBO管理的内存中获得它的数据，而具体是从哪个VBO（程序中可以有多个VBO）获取则是通过在调用glVertexAttribPointer时绑定到GL_ARRAY_BUFFER的VBO决
     * 定的。由于在调用glVertexAttribPointer之前绑定的是先前定义的VBO对象，顶点属性0现在会链接到它的顶点数据。
     */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // 前面通过调用glVertexAttribPointer，将VBO注册到顶点属性绑定的顶点缓冲对象，之后我们就可以安全地解绑VBO。
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 为了防止其他VAO的操作会影响到当前VAO，你可以解绑当前VAO。但这是很少发生的，因为如果你要操作其他VAO，
    // 必然要先绑定VAO，实际上就已经开启了其他VAO的上下文。所以一般情况下，不会主动去解绑VAO或VBO。这一步不用
    // 也可以。
    glBindVertexArray(0);
    

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        // 这里只有一个VAO，我们不用每次都绑定它。这么做是为了让代码更加规范。
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void drawRectangle() {
    GLWindow glWindow = GLWindow::Builder().setTitle("rectangle").build();

    // begin: 设置顶点着色器
    string vertexShaderCode = readShaderProgram("./shaders/hello_triangle.vs");
    if (vertexShaderCode.size() == 0) {
        printf("read vertex shader code failed\n");
        return;
    }
    const char *vertexShaderCodeCStr = vertexShaderCode.c_str();
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCodeCStr, nullptr);
    glCompileShader(vertexShader);
    

    if (!checkShaderCompileStatus(vertexShader)) {
        return;
    }
    // end: 设置顶点着色器

    // begin: 设置片段着色器
    
    string fragmentShaderCode = readShaderProgram("./shaders/hello_triangle.frag");
    if (fragmentShaderCode.size() == 0) {
        printf("read fragment shader program failed\n");
        return;
    }
    const char* fragmentShaderCodeCStr = fragmentShaderCode.c_str();
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCodeCStr, nullptr);
    glCompileShader(fragmentShader);

    if (!checkShaderCompileStatus(fragmentShader)) {
        return;
    }
    // end: 设置片段着色器

    // begin: 设置着色器程序
    GLuint shaderProgram = glCreateProgram();
    // 将shader链接到program里
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    if (!checkProgramLinkStatus(shaderProgram)) {
        return;
    }

    // 链接之后，shader就可以删除了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // end: 设置着色器程序
    

    float vertices[] = {
        -0.5f,  0.5f, 0.0f, // 左上
        -0.5f, -0.5f, 0.0f, // 左下
         0.5f, -0.5f, 0.0f, // 右下
         0.5f,  0.5f, 0.0f  // 右上
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // 这里解绑VBO是可以的，因为glVertexAttribPointer已经将VBO注册为顶点属性绑定的buffer。
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // 当目标是GL_ELEMENT_ARRAY_BUFFER的时候，VAO会存储glBindBuffer的调用，这也意味着它也会储存解绑调用，所以确保你没有在解绑VAO之前解绑索引数组缓冲，否则
    // 它就没有这个EBO配置了。
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    

    while (!glfwWindowShouldClose(glWindow.window)) {

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        
        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);

}