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
        virtual void build(UInt32 width, UInt32 height) = 0;

    protected:
        Texture(const TextureAttributes& attribute);
        UInt32 textureId;
        TextureAttributes attributes;
    };
}
