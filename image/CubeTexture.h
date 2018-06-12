#pragma once

#include "RawImage.h"
#include "Texture.h"

namespace Core {

    class CubeTexture : public Texture {
    public:
        virtual ~CubeTexture();
        virtual void build(RawImage *frontData, RawImage *backData, RawImage *topData, RawImage *bottomData, RawImage *leftData, RawImage *rightData) = 0;
    };
}
