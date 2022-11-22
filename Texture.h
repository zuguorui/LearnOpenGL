#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_

#include <stdint.h>
#include <string>

struct Texture
{
    uint32_t id;
    std::string type;
    std::string path;
};


#endif