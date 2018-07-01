#pragma once

#include "../util/WeakPointer.h"
#include "RawImage.h"
#include "Texture.h"

namespace Core {

    class CubeTexture : public Texture {
    public:
        virtual ~CubeTexture();
        virtual void build(WeakPointer<RawImage> front, WeakPointer<RawImage> back, 
                           WeakPointer<RawImage> top, WeakPointer<RawImage> bottom, 
                           WeakPointer<RawImage> left, WeakPointer<RawImage> right) = 0;

    protected:
        CubeTexture(const TextureAttributes& attributes);
    };
}
