#pragma once

#include "../image/CubeTexture.h"
#include "../image/RawImage.h"

namespace Core {

    class CubeTextureGL : public CubeTexture {
    public:
        void build(RawImage *frontData, RawImage *backData, RawImage *topData, RawImage *bottomData, RawImage *leftData, RawImage *rightData) override;
    };
}
