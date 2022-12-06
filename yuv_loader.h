#ifndef _YUV_LOADER_H_
#define _YUV_LOADER_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

bool load_yuv444p(const char *path, int width, int height, uint8_t ***yuv);

#endif