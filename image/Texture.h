#pragma once

#include <memory>

#include "../base/CoreObject.h"
#include "TextureAttr.h"
#include "../common/Exception.h"

namespace Core {

    class Texture : CoreObject {
    public:

        class TextureException: Exception {
        public:
            TextureException(const std::string& msg): Exception(msg) {}
            TextureException(const char* msg): Exception(msg) {}
        };

        virtual ~Texture();
        Int32 getTextureID() const;
        Bool isBuilt() const;
        virtual void buildEmpty(UInt32 width, UInt32 height) = 0;
        virtual void updateMipMaps() = 0;

    protected:
        Texture(const TextureAttributes& attribute);
        UInt32 textureId;
        TextureAttributes attributes;
    };
}
