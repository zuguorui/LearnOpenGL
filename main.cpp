
#include<iostream>

#include "chapter_create_window.h"
#include "chapter_hello_triangle.h"
#include "chapter_shaders.h"
#include "chapter_textures.h"
#include "chapter_transform.h"
#include "chapter_coord.h"
#include "chapter_camera.h"
#include "chapter_light.h"
#include "chapter_model.h"
#include "chapter_advanced.h"
#include "yuv2rgb.h"
#include "chapter_hdr.h"

using namespace std;

// https://learnopengl-cn.readthedocs.io/zh/latest

void be_le() {
    int16_t *a = (int16_t *)malloc(1 * sizeof(int16_t));
    a[0] = 0x1234;
    int8_t *b = (int8_t *)a;
    uint8_t c = b[0];
    uint8_t d = b[1];

    printf("c = 0x%x, d = 0x%x", c, d);
}

int main()
{
    // createWindow();
    // drawTriangle();
    // drawRectangle();
    // testShaders();
    // testUniform();
    // testMoreAttribute();
    // drawTriangleTexture();
    // drawRectangleTexture();
    // drawMixTexture();
    // testTransform();
    // draw3DCube();
    // testCamera();
    // fpsCamera();
    // testCameraClass();
    // testLight();
    // testMaterial();
    // testLightMap();
    // testParallelLight();
    // testPointLight();
    // testSpotLight();
    testLoadModel();
    // testFrameBuffer();
    // testFaceCulling();
    // yuv_to_rgb();
    // rgb565_to_rgb888();
    // display_raw_rgb565();
    // rgb444_rgb888();
    // display_dumps();
    // be_le();
    //display_hdr_image();

    
    return 0;
}
