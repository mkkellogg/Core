#pragma once

#include <memory>
#include "../common/gl.h"
#include "RawImage.h"
#include "Texture.h"

namespace Core {

    class Texture {
    public:
        virtual ~Texture();
        Int32 getTextureID();

    protected:
        Texture();
        Int32 textureId;
    };
}
