#include "RenderTarget2D.h"

namespace Core {

    RenderTarget2D::~RenderTarget2D() {
    }
    
    RenderTarget2D::RenderTarget2D(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer, const TextureAttributes& colorTextureAttributes, Vector2u size):
        RenderTarget(hasColor, hasDepth, enableStencilBuffer, colorTextureAttributes, size) {

    }
}