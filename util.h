#ifndef _UTIL_H_
#define _UTIL_H_
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

std::string readShaderProgram(const char *path);

bool checkShaderCompileStatus(unsigned int shader);

bool checkProgramLinkStatus(unsigned int program);
#endif