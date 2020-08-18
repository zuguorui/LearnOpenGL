//
//  TexturePractice.hpp
//  LearnOpenGL-xcode
//
//  Created by zhiyun on 2020/8/17.
//  Copyright © 2020 zu. All rights reserved.
//

#ifndef TexturePractice_h
#define TexturePractice_h

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>

extern "C"{
#include "stb/stb_image.h"
}


#include <glad/glad.h>
#include "WindowUtil.h"
#include "Shader.h"

using namespace std;



void drawTexture();

#endif /* TexturePractice_hpp */
