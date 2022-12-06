#include "yuv_loader.h"

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