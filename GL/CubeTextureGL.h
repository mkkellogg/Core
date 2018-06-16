#pragma once

#include "../image/CubeTexture.h"
#include "../image/RawImage.h"

namespace Core {

    // forward declaration
    class GraphicsGL;

    class CubeTextureGL final : public CubeTexture {
        friend class GraphicsGL;

    public:
        ~CubeTextureGL();
        void build(RawImage *frontData, RawImage *backData, RawImage *topData, RawImage *bottomData, RawImage *leftData, RawImage *rightData) override;

    private:
        CubeTextureGL(const TextureAttributes& attributes);
    };
}
