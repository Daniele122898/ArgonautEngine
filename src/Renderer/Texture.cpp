//
// Created by Daniele on 25/07/2021.
//

#include "Texture.h"

namespace Argonaut {

    Texture::Texture(std::string filePath, std::string type) {
        m_textureId = 0;
        m_width = m_height = m_bitDepth = 0;
        this->m_filePath = filePath;
        this->m_type = std::move(type);
    }

    Texture::Texture(std::string filePath) {
        m_textureId = 0;
        m_width = m_height = m_bitDepth = 0;
        this->m_filePath = filePath;
    }

    bool Texture::LoadTexture(bool flipImage) {
        AG_CORE_TRACE("Loading texture {}", m_filePath);
        stbi_set_flip_vertically_on_load(flipImage);
        unsigned char *data = stbi_load(m_filePath.c_str(), &m_width, &m_height, &m_bitDepth, 0);
        if (!data) {
            AG_CORE_ERROR("Failed to find texture: {0}", m_filePath);
            return false;
        }
        // TODO: parameterize these options in some form of option object or smth
        glGenTextures(1, &m_textureId);
        glBindTexture(GL_TEXTURE_2D, m_textureId);

        // Set options
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format;
        if (m_bitDepth == 1)
            format = GL_RED;
        else if (m_bitDepth == 3)
            format = GL_RGB;
        else if (m_bitDepth == 4)
            format = GL_RGBA;
        else {
            AG_CORE_ERROR("Unrecognized bit depth during image load: {0}", m_bitDepth);
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        return true;
    }

    void Texture::UseTexture() const {
        this->UseTexture(0);
    }

    void Texture::UseTexture(uint textureUnit) const {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, m_textureId);
    }

    Texture::~Texture() {
        AG_CORE_TRACE("DELETING TEXTURE {}", m_filePath);
        ClearTexture();
    }

    void Texture::ClearTexture() {
        if (m_textureId != 0) {
            glDeleteTextures(1, &m_textureId);
            m_textureId = 0;
        }
        m_filePath = (char *) "";
    }
}