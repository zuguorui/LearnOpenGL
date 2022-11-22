#include "util.h"
#include <fstream>
#include <sstream>
#include "default_gl_include.h"
#include "stb_include.h"
#include <assimp/Importer.hpp>

using namespace std;

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