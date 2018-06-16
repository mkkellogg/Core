#pragma once

#include <memory>
#include "RawImage.h"
#include "Texture.h"
#include "TextureAttr.h"

namespace Core {

    class Texture {
    public:
        virtual ~Texture();
        Int32 getTextureID();
        Bool isBuilt() const;

    protected:
        Texture(const TextureAttributes& attribute);
        Int32 textureId;
        TextureAttributes attributes;
    };
}
