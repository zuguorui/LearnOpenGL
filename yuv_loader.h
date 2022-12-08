#ifndef _YUV_LOADER_H_
#define _YUV_LOADER_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

bool load_yuv444p(int *width, int *height, int *bit_depth, uint8_t ***yuv);

// YU12 & YV12
bool load_yuv420p(int *width, int *height, int *bit_depth, uint8_t ***yuv);

// NV21 & NV12
bool load_yuv420sp(int *width, int *height, int *bit_depth, uint8_t ***yuv);

bool load_yuv420p10le(int *width, int *height, int *bit_depth, uint8_t ***yuv);

bool load_yuv420p16le(int *width, int *height, int *bit_depth, uint8_t ***yuv);

#endif