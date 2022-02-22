//
// Created by Daniele on 2/22/2022.
//

#include "Model.h"

namespace Argonaut {

    void Model::Draw(Shader &shader) {
        for (auto& mesh : m_meshes) {
            mesh.Draw(shader);
        }
    }

    void Model::loadModel(string const& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            AG_CORE_ERROR("ASSIMP ERROR: failed to load model from {} with error \n{}", path, importer.GetErrorString());
            return;
        }
        m_directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    void Model::processNode(aiNode *node, const aiScene *scene) {
        // Process all the meshes contained in this node (if any)
        for (int i = 0; i < node->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_meshes.push_back(processMesh(mesh, scene));
        }
        // then do the same for each of its children recursively
        for (int i = 0; i < node->mNumChildren; ++i) {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
        vec<Vertex> verts(mesh->mNumVertices);
        vec<uint> indx(mesh->mNumFaces*3);
        vec<Texture> tex;

        // Process vertex position, normals and texture coords
        for (int i = 0; i < mesh->mNumVertices; ++i) {
            Vertex vertex{};

            auto& vert = mesh->mVertices[i];
            vertex.Position = v3{vert.x, vert.y, vert.z};

            auto& norm = mesh->mNormals[i];
            vertex.Normal = v3{norm.x, norm.y, norm.z};

            // Check for tex coords. Assimp allows for 8 sets of texture coords
            // we don't care about that for now. Check if there's at least 1
            if (mesh->mTextureCoords[0]) {
                auto& t = mesh->mTextureCoords[0][i];
                vertex.TexCoords = v2{t.x, t.y};
            } else {
                vertex.TexCoords = v2{0.f, 0.f};
            }
            verts.push_back(vertex);
        }

        // Process indices
        for (int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace& face = mesh->mFaces[i];
            // Technically in our case this is always 3 bcs of triangulation
            // So we could speed this up by manually unrolling!
            for (int j = 0; j < face.mNumIndices; ++j) {
                indx.push_back(face.mIndices[j]);
            }
        }

        // Process material
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            vec<Texture> diffuseMaps = loadMaterialTextures(
                    material, aiTextureType_DIFFUSE, "texture_diffuse");
            tex.insert(tex.end(), diffuseMaps.begin(), diffuseMaps.end());

            vec<Texture> specularMaps = loadMaterialTextures(
                    material, aiTextureType_SPECULAR, "texture_specular");
            tex.insert(tex.end(), specularMaps.begin(), specularMaps.end());
        }

        return Mesh{verts, indx, tex};
    }

    vec<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) {
        vec<Texture> textures;
        for (int i = 0; i < mat->GetTextureCount(type); ++i) {
            aiString str;
            mat->GetTexture(type, i, &str);
            string path = fmt::format("{}/{}", m_directory, str.C_Str());

            bool found = false;
            for (auto& t: m_textures) {
                if (std::strcmp(t.GetFilePath(), path.c_str()) == 0) {
                    textures.push_back(t);
                    found = true;
                    break;
                }
            }
            if (found) continue;

            Texture tex{path.c_str(), typeName};
            tex.LoadTexture(true);
            textures.push_back(tex);
            m_textures.push_back(tex);
        }

        return textures;
    }
}