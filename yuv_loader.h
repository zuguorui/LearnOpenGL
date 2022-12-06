#ifndef _YUV_LOADER_H_
#define _YUV_LOADER_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

bool load_yuv444p(const char *path, int width, int height, uint8_t ***yuv);

// YU12 & YV12
bool load_yuv420p(const char *path, int width, int height, uint8_t ***yuv);

// NV21 & NV12
bool load_yuv420sp(const char *path, int width, int height, uint8_t ***yuv);

#endif