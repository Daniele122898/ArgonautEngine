//
// Created by Daniele on 25/07/2021.
//

#include "Texture.h"

namespace Argonaut {

    Texture::Texture(const char *filePath) {
        textureId = 0;
        width = height = bitDepth = 0;
        this->filePath = filePath;
        imageFormat = GL_RGBA;
    }

    Texture::Texture(const char *filePath, int imageFormat) {
        textureId = 0;
        width = height = bitDepth = 0;
        this->filePath = filePath;
        this->imageFormat = imageFormat;
    }

    bool Texture::LoadTexture(bool flipImage) {
        stbi_set_flip_vertically_on_load(flipImage);
        unsigned char *data = stbi_load(filePath, &width, &height, &bitDepth, 0);
        if (!data) {
            AG_CORE_ERROR("Failed to find texture: {0}", filePath);
            return false;
        }
        // TODO: parameterize these options in some form of option object or smth
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // Set options
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        return true;
    }

    void Texture::UseTexture() const {
//    glActiveTexture(0);
//    glBindTexture(GL_TEXTURE_2D, textureId);
        this->UseTexture(0);
    }

    void Texture::UseTexture(uint textureUnit) const {
        glActiveTexture(textureUnit);
        glBindTexture(GL_TEXTURE_2D, textureId);
    }

    Texture::~Texture() {
        ClearTexture();
    }

    void Texture::ClearTexture() {
        if (textureId != 0) {
            glDeleteTextures(1, &textureId);
            textureId = 0;
        }
        filePath = (char *) "";
    }
}