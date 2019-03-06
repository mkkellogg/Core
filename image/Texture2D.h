#pragma once

#include <memory>

#include "../util/WeakPointer.h"
#include "Texture.h"
#include "../image/RawImage.h"

namespace Core {

    class Texture2D: public Texture {
    public:
        virtual ~Texture2D();
        virtual void build(WeakPointer<StandardImage> imageData) = 0;
        virtual void build(WeakPointer<HDRImage> imageData) = 0;

    protected:
        Texture2D(const TextureAttributes& attributes);
    };
}
