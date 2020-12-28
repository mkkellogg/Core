#pragma once

#include <memory>

#include "../common/gl.h"
#include "../image/Texture2D.h"

namespace Core {

    // forward declaration
    class GraphicsGL;

    class Texture2DGL final : public Texture2D {
        friend class GraphicsGL;

    public:
        ~Texture2DGL() override;

        void buildFromImage(WeakPointer<StandardImage> imageData) override;
        void buildFromImage(WeakPointer<StandardImage> imageData, UInt32 resizeWidth, UInt32 resizeHeight) override;
        void buildFromImage(WeakPointer<HDRImage> imageData) override;
        void buildFromImage(WeakPointer<HDRImage> imageData, UInt32 resizeWidth, UInt32 resizeHeight) override;
        void buildFromData(Byte* data, UInt32 width, UInt32 height) override;
        void buildEmpty(UInt32 width, UInt32 height) override;
        void updateMipMaps() override;

    protected:
        Texture2DGL(const TextureAttributes& attributes);
        void setupTexture(Byte* data, UInt32 width, UInt32 height);
    };
}
