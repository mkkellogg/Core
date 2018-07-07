#pragma once

#include "RenderTarget.h"

namespace Core {

    class RenderTarget2D: public RenderTarget {
    public:
        virtual ~RenderTarget2D();

    protected:
        RenderTarget2D(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer, const TextureAttributes& colorTextureAttributes, Vector2u size);

    };

}
