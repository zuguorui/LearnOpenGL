//
//  WindowUtil.hpp
//  LearnOpenGL-xcode
//
//  Created by zhiyun on 2020/8/17.
//  Copyright © 2020 zu. All rights reserved.
//

#ifndef WindowUtil_h
#define WindowUtil_h

#include <iostream>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

//static GLFWwindow *window = NULL;

bool initWindow(GLFWwindow **window);
void destroyWindow();

#endif /* WindowUtil_hpp */
