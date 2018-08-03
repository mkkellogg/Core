#include "RenderTarget2D.h"

namespace Core {

    RenderTarget2D::~RenderTarget2D() {
    }
    
    RenderTarget2D::RenderTarget2D(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer, const TextureAttributes& colorTextureAttributes,
                                   const TextureAttributes& depthTextureAttributes, Vector2u size):
        RenderTarget(hasColor, hasDepth, enableStencilBuffer, colorTextureAttributes, depthTextureAttributes, size) {

    }
}