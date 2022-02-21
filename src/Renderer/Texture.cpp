//
// Created by Daniele on 25/07/2021.
//

#include "Texture.h"

namespace Argonaut {

    Texture::Texture(const char *filePath, std::string type) {
        m_textureId = 0;
        m_width = m_height = m_bitDepth = 0;
        this->m_filePath = filePath;
        m_imageFormat = GL_RGBA;
        this->m_type = std::move(type);
    }

    Texture::Texture(const char *filePath, int imageFormat, std::string type) {
        m_textureId = 0;
        m_width = m_height = m_bitDepth = 0;
        this->m_filePath = filePath;
        this->m_imageFormat = imageFormat;
        this->m_type = std::move(type);
    }

    bool Texture::LoadTexture(bool flipImage) {
        stbi_set_flip_vertically_on_load(flipImage);
        unsigned char *data = stbi_load(m_filePath, &m_width, &m_height, &m_bitDepth, 0);
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

        glTexImage2D(GL_TEXTURE_2D, 0, m_imageFormat, m_width, m_height, 0, m_imageFormat, GL_UNSIGNED_BYTE, data);
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