//
// Created by Daniele on 25/07/2021.
//

#ifndef ENGINE_TEXTURE_H
#define ENGINE_TEXTURE_H
#include <glad/glad.h>
#include <iostream>
#include "../../vendor/stb/stb_image.h"
#include "../Base/Types.h"
#include "../Base/Log.h"

namespace Argonaut {
    class Texture {
    public:
        explicit Texture(const char *filePath);

        Texture(const char *filePath, int imageFormat);

        bool LoadTexture(bool flipImage = false);

        void UseTexture() const;

        void UseTexture(uint textureUnit) const;

        [[nodiscard]] uint GetTextureId() const { return textureId; }

        ~Texture();

    private:
        void ClearTexture();

        uint textureId;
        int imageFormat;
        int width, height, bitDepth;

        const char *filePath;
    };
}


#endif //ENGINE_TEXTURE_H
