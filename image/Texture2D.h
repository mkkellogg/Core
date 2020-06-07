#pragma once

#include <memory>

#include "../util/WeakPointer.h"
#include "Texture.h"
#include "../image/RawImage.h"

namespace Core {

    class Texture2D: public Texture {
    public:
        virtual ~Texture2D();
        virtual void buildFromImage(WeakPointer<StandardImage> imageData) = 0;
        virtual void buildFromImage(WeakPointer<HDRImage> imageData) = 0;
        virtual void buildFromData(UInt32 width, UInt32 height, Byte* data) = 0;

    protected:
        Texture2D(const TextureAttributes& attributes);
    };
}
