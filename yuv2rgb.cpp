#include "yuv2rgb.h"

#include "pixel_loader.h"
#include "default_gl_include.h"

using namespace std;

void yuv_to_rgb() {
    

    int src_width;
    int src_height;
    int bit_depth;
    uint8_t **yuv = nullptr;

    // bool success = load_yuv444p(&src_width, &src_height, &bit_depth, &yuv);

    // bool success = load_yuv420sp(&src_width, &src_height, &bit_depth, &yuv);

    bool success = load_yuv420p10le(&src_width, &src_height, &bit_depth, &yuv);
    // bool success = load_yuv420p16le(&src_width, &src_height, &bit_depth, &yuv);

    GLWindow glWindow = GLWindow::Builder().setSize(src_width, src_height).setTitle("yuv2rgb").build();

    

    int y_width = src_width;
    int y_height = src_height;
    
    // yuv444p
    // int u_width = y_width;
    // int u_height = y_height;
    // int v_width = y_width;
    // int v_height = y_height;

    
    // yuv420p
    // nv21
    int u_width = y_width / 2;
    int u_height = y_height / 2;
    int v_width = y_width / 2;
    int v_height = y_height / 2;
    
    int dataFormat = GL_UNSIGNED_BYTE;
    if (bit_depth > 8) {
        dataFormat = GL_UNSIGNED_SHORT;
    }



    if (!success) {
        cout << "load yuv failed" << endl;
        return;
    }

    GLuint tex_y;
    glGenTextures(1, &tex_y);

    glBindTexture(GL_TEXTURE_2D, tex_y);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, y_width, y_height, 0, GL_RED, dataFormat, yuv[0]);
    glGenerateMipmap(GL_TEXTURE_2D);

    GLuint tex_u;
    glGenTextures(1, &tex_u);
    glBindTexture(GL_TEXTURE_2D, tex_u);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, u_width, u_height, 0, GL_RED, dataFormat, yuv[1]);
    glGenerateMipmap(GL_TEXTURE_2D);

    GLuint tex_v;
    glGenTextures(1, &tex_v);
    glBindTexture(GL_TEXTURE_2D, tex_v);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, v_width, v_height, 0, GL_RED, dataFormat, yuv[2]);
    glGenerateMipmap(GL_TEXTURE_2D);

    free(yuv[0]);
    free(yuv[1]);
    free(yuv[2]);
    free(yuv);


    float vertices[] = {
        // vertex pos         // tex coords
        -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, // view left-bottom to tex left-top
         1.0f, -1.0f,  0.0f,  1.0f, 1.0f, // view right-bottom to tex right-top
         1.0f,  1.0f,  0.0f,  1.0f, 0.0f, // view right-top to tex right-bottom
        -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, // view left-top to tex left-bottom
    };

    unsigned int indices[] = {
        0, 3, 2,
        2, 1, 0
    };

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    string fragmentShaderPath = "./shaders/yuv_rgb.frag";
    

    Shader mShader("./shaders/yuv_rgb.vs", fragmentShaderPath.c_str());

    mShader.use();
    mShader.setInt("tex_y", 0);
    mShader.setInt("tex_u", 1);
    mShader.setInt("tex_v", 2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_y);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex_u);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, tex_v);



    while (!glfwWindowShouldClose(glWindow.window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        mShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glDeleteTextures(1, &tex_y);
    glDeleteTextures(1, &tex_u);
    glDeleteTextures(1, &tex_v);




}



void rgb565_to_rgb888() {
    int src_width;
    int src_height;
    uint8_t *data = nullptr;
    int64_t dataSize;

    int rDepth;
    int gDepth;
    int bDepth;

    bool success = load_rgb565(&src_width, &src_height, &rDepth, &gDepth, &bDepth, &data, &dataSize);

    if (!success) {
        cout << "load data failed" << endl;
        return;
    }

    GLWindow glWindow = GLWindow::Builder().setSize(src_width, src_height).setTitle("rgb565").build();

    

    GLuint tex_rgb;
    glGenTextures(1, &tex_rgb);

    GLenum dataFormat = GL_UNSIGNED_BYTE;

    glBindTexture(GL_TEXTURE_2D, tex_rgb);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, src_width, src_height, 0, GL_RGB, dataFormat, data);
    glGenerateMipmap(GL_TEXTURE_2D);



    float vertices[] = {
        // vertex pos         // tex coords
        -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, // view left-bottom to tex left-top
         1.0f, -1.0f,  0.0f,  1.0f, 1.0f, // view right-bottom to tex right-top
         1.0f,  1.0f,  0.0f,  1.0f, 0.0f, // view right-top to tex right-bottom
        -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, // view left-top to tex left-bottom
    };

    unsigned int indices[] = {
        0, 3, 2,
        2, 1, 0
    };

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader mShader("./shaders/rgb_packet.vs", "./shaders/rgb_packet.frag");

    mShader.use();
    mShader.setInt("tex_rgb", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_rgb);

    while (!glfwWindowShouldClose(glWindow.window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        mShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glDeleteTextures(1, &tex_rgb);

}


void display_raw_rgb565() {
    int src_width;
    int src_height;
    uint8_t *data = nullptr;
    int64_t dataSize;

    int rDepth;
    int gDepth;
    int bDepth;

    bool success = load_raw_rgb565(&src_width, &src_height, &rDepth, &gDepth, &bDepth, &data, &dataSize);

    if (!success) {
        cout << "load data failed" << endl;
        return;
    }

    GLWindow glWindow = GLWindow::Builder().setSize(src_width, src_height).setTitle("rgb565").build();


    GLuint tex_rgb;
    glGenTextures(1, &tex_rgb);

    GLenum dataFormat = GL_UNSIGNED_SHORT_5_6_5;

    glBindTexture(GL_TEXTURE_2D, tex_rgb);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, src_width, src_height, 0, GL_RGB, dataFormat, data);
    glGenerateMipmap(GL_TEXTURE_2D);



    float vertices[] = {
        // vertex pos         // tex coords
        -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, // view left-bottom to tex left-top
         1.0f, -1.0f,  0.0f,  1.0f, 1.0f, // view right-bottom to tex right-top
         1.0f,  1.0f,  0.0f,  1.0f, 0.0f, // view right-top to tex right-bottom
        -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, // view left-top to tex left-bottom
    };

    unsigned int indices[] = {
        0, 3, 2,
        2, 1, 0
    };

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader mShader("./shaders/rgb_packet.vs", "./shaders/rgb_packet.frag");

    mShader.use();
    mShader.setInt("tex_rgb", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_rgb);

    while (!glfwWindowShouldClose(glWindow.window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        mShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glDeleteTextures(1, &tex_rgb);
}



void rgb444_rgb888() {
    int src_width;
    int src_height;
    uint8_t *data = nullptr;
    int64_t dataSize;

    int rDepth;
    int gDepth;
    int bDepth;

    bool success = load_rgb444(&src_width, &src_height, &rDepth, &gDepth, &bDepth, &data, &dataSize);

    if (!success) {
        cout << "load data failed" << endl;
        return;
    }

    GLWindow glWindow = GLWindow::Builder().setSize(src_width, src_height).setTitle("rgb444").build();


    GLuint tex_rgb;
    glGenTextures(1, &tex_rgb);

    GLenum dataFormat = GL_UNSIGNED_BYTE;

    glBindTexture(GL_TEXTURE_2D, tex_rgb);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, src_width, src_height, 0, GL_RGB, dataFormat, data);
    glGenerateMipmap(GL_TEXTURE_2D);



    float vertices[] = {
        // vertex pos         // tex coords
        -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, // view left-bottom to tex left-top
         1.0f, -1.0f,  0.0f,  1.0f, 1.0f, // view right-bottom to tex right-top
         1.0f,  1.0f,  0.0f,  1.0f, 0.0f, // view right-top to tex right-bottom
        -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, // view left-top to tex left-bottom
    };

    unsigned int indices[] = {
        0, 3, 2,
        2, 1, 0
    };

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader mShader("./shaders/rgb_packet.vs", "./shaders/rgb_packet.frag");

    mShader.use();
    mShader.setInt("tex_rgb", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_rgb);

    while (!glfwWindowShouldClose(glWindow.window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        mShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glDeleteTextures(1, &tex_rgb);
}

bool read_dump(const char *path, uint8_t **data, int64_t *count) {
    FILE *f = fopen(path, "rb");
    fseek(f, 0, SEEK_END);
    *count = ftell(f);
    *data = (uint8_t *)malloc(*count);

    fseek(f, 0, SEEK_SET);
    fread(*data, 1, *count, f);
    fclose(f);
    return true;
}

void display_dumps() {

    const char *y_path = "/Users/zuguorui/Downloads/dumps/y";
    const char *u_path = "/Users/zuguorui/Downloads/dumps/u";
    const char *v_path = "/Users/zuguorui/Downloads/dumps/v";

    int y_width = 1920;
    int y_height = 1080;

    int compDepth = 10;

    int u_width = y_width / 2;
    int u_height = y_height / 2;

    int v_width = y_width / 2;
    int v_height = y_height / 2;


    uint8_t *y = nullptr;
    int64_t y_size;

    uint8_t *u = nullptr;
    int64_t u_size;

    uint8_t *v = nullptr;
    int64_t v_size;

    read_dump(y_path, &y, &y_size);
    read_dump(u_path, &u, &u_size);
    read_dump(v_path, &v, &v_size);

    if (y == nullptr || u == nullptr || v == nullptr) {
        cout << "read dump failed" << endl;
        return;
    }

    GLWindow glWindow = GLWindow::Builder().setSize(y_width, y_height).setTitle("dump").build();

    GLenum dataFormat = GL_UNSIGNED_BYTE;
    if (compDepth > 8 && compDepth <= 16) {
        dataFormat = GL_UNSIGNED_SHORT;
    }

    GLuint tex_y;
    glGenTextures(1, &tex_y);

    glBindTexture(GL_TEXTURE_2D, tex_y);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, y_width, y_height, 0, GL_RED, dataFormat, y);
    glGenerateMipmap(GL_TEXTURE_2D);

    GLuint tex_u;
    glGenTextures(1, &tex_u);
    glBindTexture(GL_TEXTURE_2D, tex_u);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, u_width, u_height, 0, GL_RED, dataFormat, u);
    glGenerateMipmap(GL_TEXTURE_2D);

    GLuint tex_v;
    glGenTextures(1, &tex_v);
    glBindTexture(GL_TEXTURE_2D, tex_v);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, v_width, v_height, 0, GL_RED, dataFormat, v);
    glGenerateMipmap(GL_TEXTURE_2D);

    free(y);
    free(u);
    free(v);
    


    float vertices[] = {
        // vertex pos         // tex coords
        -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, // view left-bottom to tex left-top
         1.0f, -1.0f,  0.0f,  1.0f, 1.0f, // view right-bottom to tex right-top
         1.0f,  1.0f,  0.0f,  1.0f, 0.0f, // view right-top to tex right-bottom
        -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, // view left-top to tex left-bottom
    };

    unsigned int indices[] = {
        0, 3, 2,
        2, 1, 0
    };

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    string fragmentShaderPath = "./shaders/yuv_rgb.frag";
    

    Shader mShader("./shaders/yuv_rgb.vs", fragmentShaderPath.c_str());

    mShader.use();
    mShader.setInt("tex_y", 0);
    mShader.setInt("tex_u", 1);
    mShader.setInt("tex_v", 2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_y);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex_u);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, tex_v);



    while (!glfwWindowShouldClose(glWindow.window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        mShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(glWindow.window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glDeleteTextures(1, &tex_y);
    glDeleteTextures(1, &tex_u);
    glDeleteTextures(1, &tex_v);

}
