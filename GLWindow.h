
#ifndef _GL_WINDOW_H_
#define _GL_WINDOW_H_

#include <stdlib.h>
#include "default_gl_include.h"

class GLWindow {
public:
    GLWindow(GLWindow&& src);
    ~GLWindow();

    GLFWwindow *window;

    bool success();

    class Builder {
    public:
        ~Builder();
        Builder& setTitle(const char * title);
        Builder& setSize(int width, int height);
        Builder& setKeyCallback(GLFWkeyfun callback);
        Builder& setFrameSizeCallback(GLFWframebuffersizefun callback);
        Builder& setCursorPosCallback(GLFWcursorposfun callback);
        GLWindow build();

        
    private:
    
        char *title = nullptr;
        int width = 800;
        int height = 400;
        GLFWkeyfun keyCallback = nullptr;
        GLFWcursorposfun cursorPosCallback = nullptr;
        GLFWframebuffersizefun frameSizeCallback = nullptr;
    };

private:
    GLWindow(const char *title, int width, int height, GLFWkeyfun keyCallback, GLFWcursorposfun cursorPosCallback, 
                GLFWframebuffersizefun frameSizeCallback);
};

#endif