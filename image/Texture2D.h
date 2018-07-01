#pragma once

#include <memory>

#include "../util/WeakPointer.h"
#include "Texture.h"

namespace Core {

    //forward declarations
    class RawImage;

    class Texture2D: public Texture {
    public:
        virtual ~Texture2D();
        virtual void build(WeakPointer<RawImage> imageData) = 0;

    protected:
        Texture2D(const TextureAttributes& attributes);
    };
}
