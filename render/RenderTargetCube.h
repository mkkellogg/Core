#pragma once

#include "RenderTarget.h"

namespace Core {

    class RenderTargetCube: public RenderTarget {
    public:
        virtual ~RenderTargetCube();

    protected:
        RenderTargetCube(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer, const TextureAttributes& colorTextureAttributes, Vector2u size);
    };

}
