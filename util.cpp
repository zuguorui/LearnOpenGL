#include "util.h"
#include <fstream>
#include <sstream>
#include "default_gl_include.h"

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
