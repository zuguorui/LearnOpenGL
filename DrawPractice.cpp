//
//  DrawPractice.cpp
//  LearnOpenGL-xcode
//
//  Created by zhiyun on 2020/8/14.
//  Copyright © 2020 zu. All rights reserved.
//

#include "DrawPractice.h"
#include <math.h>
#include "WindowUtil.h"





void drawTriangle()
{
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    const char *vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "void main(){\n"
        "   gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
        "}\0";

    const char *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 color;\n"
        "uniform vec4 ourColor;\n"
        "void main(){\n"
        "   //color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "   color = ourColor;\n"
        "}\0";
    

    if(!initWindow(&window))
    {
        return;
    }
    
    
    // 初始化glew，这样才能找到对应的OpenGL函数的实现
//    glewExperimental = GL_TRUE;
//
//    glewInit();
    
    if(!gladLoadGL())
    {
        cout << "glad load GL wrong" << endl;
        return;
    }
    
    
    // 编译顶点着色器，指定点的坐标
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "Error: compile vertex shader source: " << infoLog << endl;
        return;
    }

    // 编译片段着色器，指定颜色。
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "Error: compile fragment shader source: " << infoLog << endl;
        return;
    }

    // 链接着色器程序。链接完成后就可以删除着色器了。
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "Error: link to program: " << infoLog << endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // 建立VAO和VBO。VAO是VBO及其与之相关的一系列配置的集合。只要配置好VAO，那之后就可以只用VAO就可以绘制。
    // 建立VAO和VBO的过程与c++中申请空间的步骤类似：
    // 1. glGen**（malloc申请空间）
    // 2. glBind** （将申请的空间绑定为某种类型）
    // 3. 操作
    // 4. unbind

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 首先绑定VAO，以便将VBO和对VBO的配置都存储到VAO里。
    glBindVertexArray(VAO);
    
    // 绑定VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 将顶点数据复制到缓存中供OpenGL使用
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // 设置顶点属性指针。
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        glClear(GL_COLOR_BUFFER_BIT);
        
        GLfloat timeValue = glfwGetTime();
        GLfloat redValue = (sin(timeValue) / 2) + 0.5f;
        GLfloat greenValue = (cos(timeValue) / 2) + 0.5f;
        
        GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

        glUseProgram(shaderProgram);
        
        glUniform4f(vertexColorLocation, redValue, greenValue, 0.0f, 1.0f);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    destroyWindow();
    
}


void drawRectangle()
{
    GLfloat vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };

    GLuint indices[] = { // 注意索引从0开始!
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    
    const char *vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "void main(){\n"
        "   gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
        "}\0";

    const char *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 color;\n"
        "uniform vec4 ourColor;\n"
        "void main(){\n"
        "   color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";
    

    if(!initWindow(&window))
    {
        return;
    }
    
    
    // 初始化glew，这样才能找到对应的OpenGL函数的实现
//    glewExperimental = GL_TRUE;
//
//    glewInit();
    
    if(!gladLoadGL())
    {
        cout << "glad load GL wrong" << endl;
        return;
    }
    
    
    // 编译顶点着色器，指定点的坐标
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "Error: compile vertex shader source: " << infoLog << endl;
        return;
    }

    // 编译片段着色器，指定颜色。
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "Error: compile fragment shader source: " << infoLog << endl;
        return;
    }

    // 链接着色器程序。链接完成后就可以删除着色器了。
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "Error: link to program: " << infoLog << endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    GLuint VAO, VBO, EBO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // 绑定VAO
    glBindVertexArray(VAO);
    
    // 复制顶点数组到顶点缓存中，供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // 复制索引数组到索引缓存中
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // 设置顶点属性指针。
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    glEnableVertexAttribArray(0);

    // 解绑VAO及VBO，但是不要解绑EBO，保持它与VAO的绑定
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);
    
    glPolygonMode(GL_FRONT, GL_LINE);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        glClear(GL_COLOR_BUFFER_BIT);
        
        

        glUseProgram(shaderProgram);
        
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    destroyWindow();
}

void moreProperties()
{
    GLfloat vertices[] = {
        // 位置              // 颜色
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };
    
    const char *vertexSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec3 color;\n"
        "out vec3 ourColor;\n"
        "void main(){\n"
        "   gl_Position = vec4(position, 1.0f);\n"
        "   ourColor = color;\n"
        "}\0";
    
    const char *fragmentSource =
        "#version 330 core\n"
        "in vec3 ourColor;\n"
        "out vec4 color;\n"
        "void main(){\n"
        "   color = vec4(ourColor, 1.0f);\n"
        "}\0";
    
    if(!initWindow(&window))
    {
        cout << "Error when init window" << endl;
        return;
    }
    
//    glewExperimental = true;
//    glewInit();
    
    if(!gladLoadGL())
    {
        cout << "glad load GL wrong" << endl;
        return;
    }
    
    GLint success;
    GLchar infoLog[512];
    
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    
    glCompileShader(vertexShader);
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "Error when compile vertex shader: " << infoLog << endl;
        return;
    }
    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "Error when compile fragment shader: " << infoLog << endl;
        return;
    }
    
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "Error when link program: " << infoLog << endl;
        return;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    GLuint VAO, VBO;
    
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);
    
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
    
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    destroyWindow();
}
