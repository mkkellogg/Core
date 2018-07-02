#pragma once

#include <memory>

#include "Texture.h"
#include "TextureAttr.h"

namespace Core {

    class Texture {
    public:
        virtual ~Texture();
        Int32 getTextureID() const;
        Bool isBuilt() const;

    protected:
        Texture(const TextureAttributes& attribute);
        UInt32 textureId;
        TextureAttributes attributes;
    };
}
