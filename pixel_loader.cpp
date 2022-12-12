#include "pixel_loader.h"

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
    
    readCount = fread(y, sizeof(uint16_t), y_count, f);
    if (readCount != y_count) {
        printf("read y error: expected count = %lld, read count = %lld\n", y_count, readCount);
    }

    // The higher 6 bits is padding 0. The OpenGL convert 10bit color to float by deviding UINT16_MAX.
    // The raw data will be converted to a very small float in shader and we must multuply (0xFFFF / 0x03FF)
    // to convert to normal color.
    // So we shift data to left by 6 bits to avoid lossness, and we don't need do extern job in shader, all
    // yuv format can use same shader.
    for (int i = 0; i < y_count; i++) {
        y[i] <<= 6;
    }

    readCount = fread(u, sizeof(uint16_t), u_count, f);
    if (readCount != u_count) {
        printf("read u error: expected count = %lld, read count = %lld\n", u_count, readCount);
    }

    for (int i = 0; i < u_count; i++) {
        u[i] <<= 6;
    }

    readCount = fread(v, sizeof(uint16_t), v_count, f);
    if (readCount != v_count) {
        printf("read v error: expected count = %lld, read count = %lld\n", v_count, readCount);
    }
    
    for (int i = 0; i < v_count; i++) {
        v[i] <<= 6;
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

bool load_rgb565(int *width, int *height, int *rDepth, int *gDepth, int *bDepth, uint8_t **data, int64_t *dataSize) {
    const int SRC_WIDTH = 1536;
    const int SRC_HEIGHT = 864;

    const char *path = "assets/out-1536*864-rgb565le.rgb";
    *width = SRC_WIDTH;
    *height = SRC_HEIGHT;

    FILE *f = fopen(path, "rb");
    if (f == nullptr) {
        return false;
    }


    *rDepth = 5;
    *gDepth = 6;
    *bDepth = 5;

    int64_t pixelCount = (*width) * (*height);

    *dataSize = pixelCount * 3;

    int64_t fs = ftell(f);
    fseek(f, 0, SEEK_END);
    int64_t fe = ftell(f);
    int64_t fSize = fe - fs;
    fseek(f, 0, SEEK_SET);

    uint16_t *rawData = (uint16_t *)malloc(fSize);

    fread(rawData, sizeof(int16_t), pixelCount, f);

    fseek(f, 0, SEEK_SET);

    *data = (uint8_t *)malloc(pixelCount * 3);

    const static int R_MASK = 0x00F8;
    const static int G_MASK = 0x00FC;
    const static int B_MASK = 0x00F8;

    const static int R_SHIFT = 8;
    const static int G_SHIFT = 3;
    const static int B_SHIFT = 3;

    uint16_t t;
    for (int i = 0; i < pixelCount; i++) {
        t = rawData[i];

        (*data)[3 * i] = (uint8_t)((t >> R_SHIFT) & R_MASK);
        (*data)[3 * i + 1] = (uint8_t)((t >> G_SHIFT) & G_MASK);
        (*data)[3 * i + 2] = (uint8_t)((t << B_SHIFT) & B_MASK);
    }

    fclose(f);

    return true;

}

bool load_raw_rgb565(int *width, int *height, int *rDepth, int *gDepth, int *bDepth, uint8_t **data, int64_t *dataSize) {
    const int SRC_WIDTH = 1536;
    const int SRC_HEIGHT = 864;

    const char *path = "assets/out-1536*864-rgb565le.rgb";
    *width = SRC_WIDTH;
    *height = SRC_HEIGHT;

    FILE *f = fopen(path, "rb");
    if (f == nullptr) {
        return false;
    }


    *rDepth = 5;
    *gDepth = 6;
    *bDepth = 5;


    int64_t fs = ftell(f);
    fseek(f, 0, SEEK_END);
    int64_t fe = ftell(f);
    int64_t fSize = fe - fs;
    fseek(f, 0, SEEK_SET);

    *dataSize = fSize;

    *data = (uint8_t *)malloc(fSize);

    fread(*data, 1, fSize, f);

    fclose(f);

    return true;
}

