#include "yuv_loader.h"

#include <algorithm>

using namespace std;



bool load_yuv444p(int *width, int *height, int *bit_depth, uint8_t ***yuv) {
    const int SRC_WIDTH = 1536;
    const int SRC_HEIGHT = 864;

    const char *path = "assets/out-1536*864-444p.yuv";
    *width = SRC_WIDTH;
    *height = SRC_HEIGHT;
    *bit_depth = 8;

    FILE *f = fopen(path, "rb");
    if (f == nullptr) {
        return false;
    }

    int64_t pixelCount = (*width) * (*height);

    uint8_t *y = (uint8_t *)malloc(pixelCount * sizeof(uint8_t));
    uint8_t *u = (uint8_t *)malloc(pixelCount * sizeof(uint8_t));
    uint8_t *v = (uint8_t *)malloc(pixelCount * sizeof(uint8_t));
    
    fread(y, sizeof(int8_t), pixelCount, f);
    fread(u, sizeof(int8_t), pixelCount, f);
    fread(v, sizeof(int8_t), pixelCount, f);

    fclose(f);
    

    *yuv = (uint8_t **)malloc(3 * sizeof(uint8_t *));
    (*yuv)[0] = y;
    (*yuv)[1] = u;
    (*yuv)[2] = v;

    return true;
}


bool load_yuv420p(int *width, int *height, int *bit_depth, uint8_t ***yuv) {

    const int SRC_WIDTH = 1536;
    const int SRC_HEIGHT = 864;

    const char *path = "assets/out-1536*864-420p.yuv";
    *width = SRC_WIDTH;
    *height = SRC_HEIGHT;
    *bit_depth = 8;

    FILE *f = fopen(path, "rb");
    if (f == nullptr) {
        return false;
    }

    int64_t pixelCount = (*width) * (*height);

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

bool load_yuv420sp(int *width, int *height, int *bit_depth, uint8_t ***yuv) {

    const int SRC_WIDTH = 1536;
    const int SRC_HEIGHT = 864;

    const char *path = "assets/out-1536*864-nv21.yuv";
    *width = SRC_WIDTH;
    *height = SRC_HEIGHT;
    *bit_depth = 8;


    FILE *f = fopen(path, "rb");
    if (f == nullptr) {
        return false;
    }

    int64_t pixelCount = (*width) * (*height);

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

bool load_yuv420p10le(int *width, int *height, int *bit_depth, uint8_t ***yuv) {
    const int SRC_WIDTH = 1536;
    const int SRC_HEIGHT = 864;

    const char *path = "assets/out-1536*864-yuv420p10le.yuv";
    *width = SRC_WIDTH;
    *height = SRC_HEIGHT;
    *bit_depth = 10;

    FILE *f = fopen(path, "rb");
    if (f == nullptr) {
        return false;
    }

    int64_t fs = ftell(f);
    fseek(f, 0, SEEK_END);
    int64_t fe = ftell(f);
    int64_t fSize = fe - fs;
    fseek(f, 0, SEEK_SET);

    int64_t pixelCount = (*width) * (*height);

    int64_t cfSize = pixelCount * (16 + 8) / 8;

    printf("fSize = %lld, cfSize = %lld\n", fSize, cfSize);

    int64_t y_count = pixelCount;
    int64_t u_count = pixelCount / 4;
    int64_t v_count = pixelCount / 4;


    uint16_t *y = (uint16_t *)malloc(y_count * sizeof(uint16_t));
    uint16_t *u = (uint16_t *)malloc(u_count * sizeof(uint16_t));
    uint16_t *v = (uint16_t *)malloc(v_count * sizeof(uint16_t));

    // YU12 -> YYYYYYYYUUVV
    // YV12 -> YYYYYYYYVVUU

    int64_t readCount = 0;
    // uint16_t x;
    // for (int i = 0; i < y_count; i++) {
    //     fread(&x, sizeof(uint16_t), 1, f);
    //     x <<= 6;
    //     y[i] = x;
    // }
    readCount = fread(y, sizeof(uint16_t), y_count, f);
    if (readCount != y_count) {
        printf("read y error: expected count = %lld, read count = %lld\n", y_count, readCount);
    }

    // for (int i = 0; i < u_count; i++) {
    //     fread(&x, sizeof(uint16_t), 1, f);
    //     x <<= 6;
    //     u[i] = x;
    // }
    readCount = fread(u, sizeof(uint16_t), u_count, f);
    if (readCount != u_count) {
        printf("read u error: expected count = %lld, read count = %lld\n", u_count, readCount);
    }

    // for (int i = 0; i < v_count; i++) {
    //     fread(&x, sizeof(uint16_t), 1, f);
    //     x <<= 6;
    //     v[i] = x;
    // }

    readCount = fread(v, sizeof(uint16_t), v_count, f);
    if (readCount != v_count) {
        printf("read v error: expected count = %lld, read count = %lld\n", v_count, readCount);
    }
    

    fclose(f);
    

    *yuv = (uint8_t **)malloc(3 * sizeof(uint8_t *));
    (*yuv)[0] = (uint8_t *)y;
    (*yuv)[1] = (uint8_t *)u;
    (*yuv)[2] = (uint8_t *)v;

    return true;
}

bool load_yuv420p16le(int *width, int *height, int *bit_depth, uint8_t ***yuv) {
    const int SRC_WIDTH = 1536;
    const int SRC_HEIGHT = 864;

    const char *path = "assets/out-1536*864-yuv420p16le.yuv";
    *width = SRC_WIDTH;
    *height = SRC_HEIGHT;
    *bit_depth = 16;

    FILE *f = fopen(path, "rb");
    if (f == nullptr) {
        return false;
    }

    int64_t fs = ftell(f);
    fseek(f, 0, SEEK_END);
    int64_t fe = ftell(f);
    int64_t fSize = fe - fs;
    fseek(f, 0, SEEK_SET);

    int64_t pixelCount = (*width) * (*height);

    int64_t cfSize = pixelCount * (16 + 8) / 8;

    printf("fSize = %lld, cfSize = %lld\n", fSize, cfSize);

    int64_t y_count = pixelCount;
    int64_t u_count = pixelCount / 4;
    int64_t v_count = pixelCount / 4;


    uint16_t *y = (uint16_t *)malloc(y_count * sizeof(uint16_t));
    uint16_t *u = (uint16_t *)malloc(u_count * sizeof(uint16_t));
    uint16_t *v = (uint16_t *)malloc(v_count * sizeof(uint16_t));

    // YU12 -> YYYYYYYYUUVV
    // YV12 -> YYYYYYYYVVUU

    int64_t readCount = 0;

    readCount = fread(y, sizeof(uint16_t), y_count, f);
    if (readCount != y_count) {
        printf("read y error: expected count = %lld, read count = %lld\n", y_count, readCount);
    }

    readCount = fread(u, sizeof(uint16_t), u_count, f);
    if (readCount != u_count) {
        printf("read u error: expected count = %lld, read count = %lld\n", u_count, readCount);
    }

    readCount = fread(v, sizeof(uint16_t), v_count, f);
    if (readCount != v_count) {
        printf("read v error: expected count = %lld, read count = %lld\n", v_count, readCount);
    }
    

    fclose(f);
    

    *yuv = (uint8_t **)malloc(3 * sizeof(uint8_t *));
    (*yuv)[0] = (uint8_t *)y;
    (*yuv)[1] = (uint8_t *)u;
    (*yuv)[2] = (uint8_t *)v;

    return true;
}