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
        void build(UInt32 width, UInt32 height) override;

    private:
        CubeTextureGL(const TextureAttributes& attributes);
        void setupTexture(UInt32 width, UInt32 height, Byte* front, Byte* back, Byte* top, Byte* bottom, Byte* left, Byte* right);
    };
}
