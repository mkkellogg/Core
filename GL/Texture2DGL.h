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
        ~Texture2DGL();

        void build(WeakPointer<StandardImage> imageData) override;
        void build(UInt32 width, UInt32 height) override;

    protected:
        Texture2DGL(const TextureAttributes& attributes);
        void setupTexture(UInt32 width, UInt32 height, Byte* data);
    };
}
