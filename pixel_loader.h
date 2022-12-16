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

bool load_rgb565(int *width, int *height, int *rDepth, int *gDepth, int *bDepth, uint8_t **data, int64_t *dataSize);

bool load_raw_rgb565(int *width, int *height, int *rDepth, int *gDepth, int *bDepth, uint8_t **data, int64_t *dataSize);

bool load_rgb444(int *width, int *height, int *rDepth, int *gDepth, int *bDepth, uint8_t **data, int64_t *dataSize);

bool load_raw_rgb444(int *width, int *height, int *rDepth, int *gDepth, int *bDepth, uint8_t **data, int64_t *dataSize);

#endif