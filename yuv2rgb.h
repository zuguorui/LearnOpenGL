#ifndef _YUV_TO_RGB_H_
#define _YUV_TO_RGB_H_

#include "GLWindow.h"
#include "Shader.h"
#include "default_stb_include.h"
#include "default_gl_include.h"
#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>


void yuv_to_rgb();

void rgb565_to_rgb888();

void display_raw_rgb565();

void rgb444_rgb888();

void display_dumps();

#endif