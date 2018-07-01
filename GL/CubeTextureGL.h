#pragma once

#include "../image/CubeTexture.h"

namespace Core {

    // forward declaration
    class GraphicsGL;
    class RawImage;

    class CubeTextureGL final : public CubeTexture {
        friend class GraphicsGL;

    public:
        ~CubeTextureGL();
        void build(WeakPointer<RawImage> frontData, WeakPointer<RawImage> backData, 
                   WeakPointer<RawImage> topData,WeakPointer<RawImage> bottomData, 
                   WeakPointer<RawImage> leftData, WeakPointer<RawImage> rightData) override;

    private:
        CubeTextureGL(const TextureAttributes& attributes);
    };
}
