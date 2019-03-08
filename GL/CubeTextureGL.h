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
        void buildFromImages(WeakPointer<StandardImage> frontData, WeakPointer<StandardImage> backData, 
                             WeakPointer<StandardImage> topData,WeakPointer<StandardImage> bottomData, 
                             WeakPointer<StandardImage> leftData, WeakPointer<StandardImage> rightData) override;
        void buildFromImages(WeakPointer<HDRImage> frontData, WeakPointer<HDRImage> backData, 
                             WeakPointer<HDRImage> topData,WeakPointer<HDRImage> bottomData, 
                             WeakPointer<HDRImage> leftData, WeakPointer<HDRImage> rightData) override;
        void buildEmpty(UInt32 width, UInt32 height) override;

    private:
        CubeTextureGL(const TextureAttributes& attributes);
        void setupTexture(UInt32 width, UInt32 height, Byte* front, Byte* back, Byte* top, Byte* bottom, Byte* left, Byte* right);
    };
}
