#ifndef _UTIL_H_
#define _UTIL_H_
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "default_gl_include.h"
#include <assimp/texture.h>

std::string readShaderProgram(const char *path);

bool checkShaderCompileStatus(unsigned int shader);

bool checkProgramLinkStatus(unsigned int program);

GLuint loadTexture(const char *path, bool flipVertically = true, int wrapS = GL_CLAMP_TO_EDGE, int wrapT = GL_CLAMP_TO_EDGE, GLint magFilterMode = GL_NEAREST, GLint minFilterMode = GL_LINEAR);

GLuint loadTextureFromAssimp(const aiTexture* aiTex, bool flipVertically = true, int wrapS = GL_CLAMP_TO_EDGE, int wrapT = GL_CLAMP_TO_EDGE, GLint magFilterMode = GL_NEAREST, GLint minFilterMode = GL_LINEAR);



#endif