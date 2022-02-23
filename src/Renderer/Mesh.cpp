//
// Created by Daniele on 2/21/2022.
//

#include "Mesh.h"
#include <fmt/core.h>

namespace Argonaut {

    void Mesh::setupMesh() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, (m_vertices.size() * sizeof(Vertex)),
                     &m_vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint),
                     &m_indices[0], GL_STATIC_DRAW);

        // Vertex positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        // Vertex Normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, Normal));
        glEnableVertexAttribArray(1);

        // Vertex Texture coords
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, TexCoords));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    void Mesh::Draw(Shader &shader) {
        uint diffNr = 1;
        uint specNr = 1;
        for (uint i = 0; i < m_textures.size(); ++i) {
            auto& tex = m_textures[i];
            auto typ = tex->GetType();
            std::string num;

            if (typ == "texture_diffuse")
                num = std::to_string(diffNr++);
            else if (typ == "texture_specular")
                num = std::to_string(specNr++);
            else {
                AG_CORE_ERROR("UNKNOWN MESH TEXTURE TYPE: {}", typ);
            }

            shader.setFloat(
                    fmt::format("material.{}{}", typ,num),
                    static_cast<float>(i));

            tex->UseTexture(i);
        }
        glActiveTexture(GL_TEXTURE0);

        // Draw Mesh
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}