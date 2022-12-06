#include "yuv_loader.h"

#include <algorithm>

using namespace std;

bool load_yuv444p(const char *path, int width, int height, uint8_t ***yuv) {
    FILE *f = fopen(path, "rb");
    if (f == nullptr) {
        return false;
    }

    int64_t pixelCount = width * height;

    uint8_t *y = (uint8_t *)malloc(pixelCount * sizeof(uint8_t));
    uint8_t *u = (uint8_t *)malloc(pixelCount * sizeof(uint8_t));
    uint8_t *v = (uint8_t *)malloc(pixelCount * sizeof(uint8_t));

    fread(y, 1, pixelCount * sizeof(uint8_t), f);
    fread(u, 1, pixelCount * sizeof(uint8_t), f);
    fread(v, 1, pixelCount * sizeof(uint8_t), f);

    fclose(f);
    

    *yuv = (uint8_t **)malloc(3 * sizeof(uint8_t *));
    (*yuv)[0] = y;
    (*yuv)[1] = u;
    (*yuv)[2] = v;

    return true;
}


bool load_yuv420p(const char *path, int width, int height, uint8_t ***yuv) {
    FILE *f = fopen(path, "rb");
    if (f == nullptr) {
        return false;
    }

    int64_t pixelCount = width * height;

    int64_t y_count = pixelCount;
    int64_t u_count = pixelCount / 4;
    int64_t v_count = pixelCount / 4;


    uint8_t *y = (uint8_t *)malloc(y_count * sizeof(uint8_t));
    uint8_t *u = (uint8_t *)malloc(u_count * sizeof(uint8_t));
    uint8_t *v = (uint8_t *)malloc(v_count * sizeof(uint8_t));

    // YU12 -> YYYYYYYYUUVV
    // YV12 -> YYYYYYYYVVUU
    fread(y, 1, y_count * sizeof(uint8_t), f);
    fread(u, 1, u_count * sizeof(uint8_t), f);
    fread(v, 1, v_count * sizeof(uint8_t), f);
    

    fclose(f);
    

    *yuv = (uint8_t **)malloc(3 * sizeof(uint8_t *));
    (*yuv)[0] = y;
    (*yuv)[1] = u;
    (*yuv)[2] = v;

    return true;
}

bool load_yuv420sp(const char *path, int width, int height, uint8_t ***yuv) {
    FILE *f = fopen(path, "rb");
    if (f == nullptr) {
        return false;
    }

    int64_t pixelCount = width * height;

    int64_t y_count = pixelCount;
    int64_t u_count = pixelCount / 4;
    int64_t v_count = pixelCount / 4;


    uint8_t *y = (uint8_t *)malloc(y_count * sizeof(uint8_t));
    uint8_t *u = (uint8_t *)malloc(u_count * sizeof(uint8_t));
    uint8_t *v = (uint8_t *)malloc(v_count * sizeof(uint8_t));

    // NV12 -> YYYYYYYYUVUV
    // NV21 -> YYYYYYYYVUVU
    fread(y, 1, y_count * sizeof(uint8_t), f);

    for (int i = 0; i < u_count; i++) {
        fread(v + i, 1, 1, f);
        fread(u + i, 1, 1, f);
    }
    
    

    fclose(f);
    

    *yuv = (uint8_t **)malloc(3 * sizeof(uint8_t *));
    (*yuv)[0] = y;
    (*yuv)[1] = u;
    (*yuv)[2] = v;

    return true;
}