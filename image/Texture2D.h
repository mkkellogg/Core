#pragma once

#include <memory>

#include "Texture.h"

namespace Core {

    //forward declarations
    class RawImage;

    class Texture2D: public Texture {
    public:
        virtual ~Texture2D();
        virtual void build(RawImage *imageData) = 0;

    protected:
        Texture2D(const TextureAttributes& attributes);
    };
}
