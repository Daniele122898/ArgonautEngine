//
// Created by Daniele on 25/07/2021.
//

#ifndef ENGINE_TEXTURE_H
#define ENGINE_TEXTURE_H
#include <glad/glad.h>
#include <iostream>
#include <utility>

#include "../../vendor/stb/stb_image.h"
#include "../Base/Types.h"
#include "../Base/Log.h"


namespace Argonaut {
    class Texture {
    public:
        explicit Texture(const char *filePath, std::string type);

        Texture(const char *filePath, int imageFormat, std::string type);

        bool LoadTexture(bool flipImage = false);

        void UseTexture() const;

        void UseTexture(uint textureUnit) const;

        [[nodiscard]]
        uint GetTextureId() const { return m_textureId; }

        [[nodiscard]]
        const char* GetFilePath() const {return m_filePath; }

        [[nodiscard]]
        std::string_view GetType() const { return m_type; }

        ~Texture();

    private:
        void ClearTexture();

        uint m_textureId;
        int m_imageFormat;
        int m_width, m_height, m_bitDepth;

        std::string m_type;

        const char* m_filePath;
    };
}


#endif //ENGINE_TEXTURE_H
