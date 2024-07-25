#include "util.h"
#include <fstream>
#include <sstream>
#include "default_gl_include.h"
#include "default_stb_include.h"
#include <assimp/Importer.hpp>



using namespace std;
using namespace Imf;
using namespace Imath;

string readShaderProgram(const char* path) {
    
    ifstream f;
    f.exceptions(ifstream::badbit | ifstream::failbit);
    
    try {
        f.open(path);
        stringstream ss;
        ss << f.rdbuf();
        f.close();
        string s = ss.str();
        return s;
    } catch(ifstream::failure e) {
        cout << "failed to load program: " << path << endl;
    }
    return "";
}

char info[512];
int ret;

bool checkShaderCompileStatus(unsigned int shader) {
    ret = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ret);
    if (!ret) {
        glGetShaderInfoLog(shader, 512, nullptr, info);
        printf("gl shader compile failed, log = %s\n", info);
        return false;
    }
    return true;
}


bool checkProgramLinkStatus(unsigned int program) {
    ret = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &ret);
    if (!ret) {
        glGetProgramInfoLog(program, 512, nullptr, info);
        printf("gl program link failed, log = %s", info);
        return false;
    }
    return true;
}

unsigned int loadTexture(const char *path, bool flipVertically, int wrapS, int wrapT, GLint magFilterMode, GLint minFilterMode) {
    int width, height, numChannels;
    uint8_t *data = nullptr;
    stbi_set_flip_vertically_on_load(flipVertically);
    data = stbi_load(path, &width, &height, &numChannels, 0);

    if (!data) {
        cout << "load image failed" << endl;
        return 0;
    }

    GLenum format = GL_RGB;
    if (numChannels == 1) {
        numChannels = GL_RED;
    } else if (numChannels == 3) {
        format = GL_RGB;
    } else if (numChannels == 4) {
        format = GL_RGBA;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilterMode);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    

    stbi_image_free(data);
    return texture;
}

GLuint loadTextureFromAssimp(const aiTexture* aiTex, bool flipVertically, int wrapS, int wrapT, GLint magFilterMode, GLint minFilterMode) {
    if (aiTex == nullptr) {
        return 0;
    }
    int width, height, numChannels;
    uint8_t *data = nullptr;
    stbi_set_flip_vertically_on_load(flipVertically);

    if (aiTex->mHeight == 0) {
        data = stbi_load_from_memory(reinterpret_cast<uint8_t *>(aiTex->pcData), aiTex->mWidth, &width, &height, &numChannels, 0);
    } else {
        data = stbi_load_from_memory(reinterpret_cast<uint8_t *>(aiTex->pcData), aiTex->mWidth * aiTex->mHeight, &width, &height, &numChannels, 0);
    }

    GLenum format = GL_RGB;
    if (numChannels == 1) {
        numChannels = GL_RED;
    } else if (numChannels == 3) {
        format = GL_RGB;
    } else if (numChannels == 4) {
        format = GL_RGBA;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilterMode);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}


void read_hdr_file(const char *path, int &width, int &height, Array2D<float> &rPix, Array2D<float> &gPix, Array2D<float> &bPix) {
    InputFile f(path);
    Box2i dataWindow = f.header().dataWindow();
    width = dataWindow.max.x - dataWindow.min.x + 1;
    height = dataWindow.max.y - dataWindow.min.y + 1;

    // 注意这里，使用height作为Array2D的宽，使用width作为Array2D的高，这是因为传统
    // 数学上二维数组和图像的访问方式不一致（或者说方向不一样）。因为二维数组的访问方式
    // 是[xIndex][yIndex]，先行后列。而图像像素是横向排布的，
    // 图像坐标原点在左上角，x向右，y轴向下，尽管它也是先取行，但是这里的行坐标却是yIndex。
    // 按照先行后列的原则，就需要使二维数组的原点与图像坐标原点重合，二维数组的x轴为图像坐标的y轴，
    // 二维数组的y轴为图像坐标的x轴。因此图像的宽高与二维数组的宽高恰好是颠倒的。
    rPix.resizeErase(height, width);
    gPix.resizeErase(height, width);
    bPix.resizeErase(height, width);
    
    FrameBuffer frameBuffer;

    // 注意base的计算。由于dataWindow并非是完全符合从[0, 0]到[width, height]的矩形，
    // 它的左上角是[dataWindow.min.x, dataWindow.min.y]，右下角是[dataWindow.max.x, dataWindow.max.y]。
    // 在OpenEXR中，像素实际上是存储在一个一维数组中（也就是Array2D内部维护的是一维数组）不过是按照二维方式访问。
    // 为了不浪费多余空间，我们要将[dataWindow.min.x, dataWindow.min.y]与[0, 0]对齐。所以设定一个虚假的地址
    // base，使得当我们按照[dataWindow.min.x, dataWindow.min.y]到[dataWindow.min.x + width, dataWindow.min.y + height]
    // 范围内访问像素时，能够映射到实际的[0, 0]到[width, height]范围内。
    // 假设ptr是一维数组地址，访问[x, y]位置的像素时是ptr + y * width + x。
    // 而假设对齐后的起始地址，也就是base，当访问[dataWindow.min.x, dataWindow.min.y]位置的像素时，应该是
    // 访问实际一维数组的起始地址，也就是ptr = base + dataWindow.min.y * width + dataWindow.min.x。
    // 因此base = ptr - dataWindow.min.y * width - dataWindow.min.x。
    frameBuffer.insert(
        "R", // name
        Slice(
            PixelType::FLOAT, // pixel type
            (char *)(&rPix[0][0] - dataWindow.min.x - dataWindow.min.y * width), // base
            sizeof(rPix[0][0]), // xStride
            sizeof(rPix[0][0]) * width, // yStride
            1, // xSampling
            1, // ySampling
            0.0f // default value
        )
    );

    frameBuffer.insert(
        "G",
        Slice(
            PixelType::FLOAT,
            (char *)(&gPix[0][0] - dataWindow.min.x - dataWindow.min.y * width),
            sizeof(gPix[0][0]),
            sizeof(gPix[0][0]) * width,
            1,
            1,
            0.0f
        )
    );

    frameBuffer.insert(
        "B",
        Slice(
            PixelType::FLOAT,
            (char *)(&bPix[0][0] - dataWindow.min.x - dataWindow.min.y * width),
            sizeof(bPix[0][0]),
            sizeof(bPix[0][0]) * width,
            1,
            1,
            0.0f
        )
    );

    f.setFrameBuffer(frameBuffer);
    f.readPixels(dataWindow.min.y, dataWindow.max.y);
}
