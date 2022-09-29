#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <iostream>
#include <stdlib.h>

#include "stb_include.h"


#include "glad/glad.h"
#include "WindowUtil.h"
#include "Shader.h"

using namespace std;

class Transform
{
public:
    void draw();

private:
    GLFWwindow *window = NULL;
};
#endif