#ifndef _UTIL_H_
#define _UTIL_H_
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "default_gl_include.h"
#include <assimp/texture.h>
#include <OpenEXR/ImfRgbaFile.h>
#include <OpenEXR/ImfBoxAttribute.h>
#include <OpenEXR/ImfArray.h>
#include <Imath/ImathBox.h>
#include <OpenEXR/ImfInputFile.h>

std::string readShaderProgram(const char *path);

bool checkShaderCompileStatus(unsigned int shader);

bool checkProgramLinkStatus(unsigned int program);

GLuint loadTexture(const char *path, bool flipVertically = true, int wrapS = GL_CLAMP_TO_EDGE, int wrapT = GL_CLAMP_TO_EDGE, GLint magFilterMode = GL_NEAREST, GLint minFilterMode = GL_LINEAR);

GLuint loadTextureFromAssimp(const aiTexture* aiTex, bool flipVertically = true, int wrapS = GL_CLAMP_TO_EDGE, int wrapT = GL_CLAMP_TO_EDGE, GLint magFilterMode = GL_NEAREST, GLint minFilterMode = GL_LINEAR);

void read_hdr_file(const char *path, int &width, int &height, Imf::Array2D<float> &rPix, Imf::Array2D<float> &gPix, Imf::Array2D<float> &bPix);

#endif