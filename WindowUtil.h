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

#include "default_gl_include.h"



//static GLFWwindow *window = NULL;

bool initWindow(GLFWwindow **window);
void destroyWindow();

#endif /* WindowUtil_hpp */
