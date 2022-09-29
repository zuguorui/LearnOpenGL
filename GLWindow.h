
#ifndef _GL_WINDOW_H_
#define _GL_WINDOW_H_

#include <stdlib.h>
#include "default_gl_include.h"

class GLWindow {
public:
    ~GLWindow();

    GLWindow *window;

    static class Builder {
    public:
        ~Builder();
        Builder& setTitle(const char * title);
        Builder& setSize(int width, int height);
        Builder& setKeyCallback(GLFWkeyfun callback);
        Builder& setFrameSizeCallback(GLFWframebuffersizefun callback);
        GLWindow *build();
    private:
    
        char *title = nullptr;
        int width = 800;
        int height = 400;
        GLFWkeyfun keyCallback = nullptr;
        GLFWframebuffersizefun frameSizeCallback = nullptr;
    };

private:
    GLWindow(const char *title, int width, int height, GLFWkeyfun keyCallback, GLFWframebuffersizefun frameSizeCallback);
};

#endif