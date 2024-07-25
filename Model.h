#ifndef _MODEL_H_
#define _MODEL_H_

#include "Mesh.h"
#include "RenderProgram.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <stdlib.h>
#include <map>

class Model {
public:
    Model(std::string path);
    void draw(RenderProgram &shader);
private:
    std::vector<Mesh> meshes;
    std::map<std::string, Texture> loadedTextures;
    std::string directory;
    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Texture> loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName, const aiScene *scene);
};

#endif