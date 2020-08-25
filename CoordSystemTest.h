#pragma once

#include <iostream>
#include <stdlib.h>
#include "stb_include.h"
#include "WindowUtil.h"
#include "Shader.h"

using namespace std;

class CoordSystemTest
{
public:
    void draw();

private:
    GLFWwindow *window = NULL;
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
};

