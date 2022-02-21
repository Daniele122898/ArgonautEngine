//
// Created by Daniele on 2/21/2022.
//

#ifndef ENGINE_MESH_H
#define ENGINE_MESH_H

#include <string>
#include <utility>
#include <vector>
#include <glad/glad.h>

#include "../Base/Types.h"
#include "Shader.h"
#include "Texture.h"

namespace Argonaut {
    struct Vertex {
        v3 Position;
        v3 Normal;
        v2 TexCoords;
    };

//    struct Texture {
//        uint id;
//        std::string type;
//    };

    class Mesh {
        // Mesh data
        std::vector<Vertex> m_vertices;
        std::vector<uint> m_indices;
        std::vector<Texture> m_textures;

        Mesh(vec<Vertex> vertices, vec<uint> indices, vec<Texture> textures):
        m_vertices{std::move(vertices)}, m_indices{std::move(indices)},
        m_textures{std::move(textures)} {
            setupMesh();
        };

        void Draw(Shader& shader);

    private:
        // Render data
        uint m_VAO, m_VBO, m_EBO;

        void setupMesh();
    };
}

#endif //ENGINE_MESH_H
