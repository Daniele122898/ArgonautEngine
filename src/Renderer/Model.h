//
// Created by Daniele on 2/22/2022.
//

#ifndef ENGINE_MODEL_H
#define ENGINE_MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Base/Types.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "../Base/Log.h"

namespace Argonaut {
    class Model {
    public:
        explicit Model(const char* path) {
            loadModel(path);
        }

        void Draw(Shader& shader);
    private:
        vec<Mesh> m_meshes;
        vec<std::shared_ptr<Texture>> m_textures;
        string m_directory;

        void loadModel(string const& path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        vec<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    };
}

#endif //ENGINE_MODEL_H
