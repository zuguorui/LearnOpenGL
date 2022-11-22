#include "Model.h"
#include "util.h"
#include <set>

using namespace std;
using namespace Assimp;

Model::Model(string path) {
    loadModel(path);
}

void Model::draw(Shader &shader) {
    for (int i = 0; i < meshes.size(); i++) {
        meshes[i].draw(shader);
    }
}

void Model::loadModel(string path) {
    Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for (int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    vector<Vertex> vertices;
    vector<uint32_t> indices;
    vector<Texture> textures;

    for (int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        if (mesh->mTextureCoords[0]) {
            vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
        } else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }
        

        vertices.push_back(vertex);
    }

    for (int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);

        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", scene);

        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    }

    return Mesh(vertices, indices, textures);

}


vector<Texture> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, string typeName, const aiScene *scene) {
    vector<Texture> textures;
    
    for (int i = 0; i < material->GetTextureCount(type); i++) {
        aiTextureMapMode uMapMode = aiTextureMapMode::aiTextureMapMode_Wrap;
        material->Get(AI_MATKEY_MAPPINGMODE_U(type, i), uMapMode);
        aiTextureMapMode vMapMode = aiTextureMapMode::aiTextureMapMode_Wrap;
        material->Get(AI_MATKEY_MAPPINGMODE_V(type, i), vMapMode);
        cout << "vMode:" << vMapMode << ", uMode:" << uMapMode << endl;
        aiString str;
        material->GetTexture(type, i, &str);
        string fullPath = (directory + "/" + str.C_Str());
        
        auto loaded = loadedTextures.find(fullPath);
        if (loaded != loadedTextures.end()) {
            textures.push_back(loaded->second);
            continue;
        }
        
        Texture texture;

        int wrapS = GL_CLAMP_TO_BORDER;
        if (uMapMode == aiTextureMapMode::aiTextureMapMode_Wrap) {
            wrapS = GL_INCR_WRAP;
        } else if (uMapMode == aiTextureMapMode::aiTextureMapMode_Mirror) {
            wrapS = GL_MIRRORED_REPEAT;
        } else if (uMapMode == aiTextureMapMode::aiTextureMapMode_Decal) {
            wrapS = GL_DECR_WRAP;
        }

        int wrapT = GL_CLAMP_TO_BORDER;
        if (vMapMode == aiTextureMapMode::aiTextureMapMode_Wrap) {
            wrapT = GL_INCR_WRAP;
        } else if (vMapMode == aiTextureMapMode::aiTextureMapMode_Mirror) {
            wrapT = GL_MIRRORED_REPEAT;
        } else if (vMapMode == aiTextureMapMode::aiTextureMapMode_Decal) {
            wrapT = GL_DECR_WRAP;
        }



        const aiTexture *aiTex = scene->GetEmbeddedTexture(str.C_Str());
        if (aiTex) {
            texture.id = loadTextureFromAssimp(aiTex, false, wrapS, wrapT);
        } else {
            texture.id = loadTexture(fullPath.c_str(), false, wrapS, wrapT);
        }

        texture.type = typeName;
        texture.path = fullPath;

        loadedTextures[fullPath] = texture;
        textures.push_back(texture);
    }
    return textures;
}
