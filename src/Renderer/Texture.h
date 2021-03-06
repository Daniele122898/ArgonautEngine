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
        explicit Texture(std::string filePath);
        explicit Texture(std::string filePath, std::string type);

        Texture(const Texture& texture) = delete;
        Texture(Texture&& texture) = delete;
        Texture& operator=(const Texture& texture) = delete;

        bool LoadTexture(bool flipImage = false);

        void UseTexture() const;

        void UseTexture(uint textureUnit) const;

        [[nodiscard]]
        uint GetTextureId() const { return m_textureId; }

        [[nodiscard]]
        std::string_view GetFilePath() const {return m_filePath; }

        [[nodiscard]]
        std::string_view GetType() const { return m_type; }

        ~Texture();

    private:
        void ClearTexture();

        uint m_textureId;
        int m_width, m_height, m_bitDepth;

        std::string m_type;

        std::string m_filePath;
    };
}


#endif //ENGINE_TEXTURE_H
