#ifndef _MESH_H_
#define _MESH_H_

#include <iostream>
#include <stdlib.h>
#include <vector>

#include "Vertex.h"
#include "Texture.h"
#include "Shader.h"
#include "default_gl_include.h"

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);

    void draw(Shader &shader);

private:
    uint32_t VAO, VBO, EBO;

    void setupMesh();
};

#endif