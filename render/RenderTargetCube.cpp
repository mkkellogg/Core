#include "RenderTargetCube.h"

namespace Core {

    RenderTargetCube::~RenderTargetCube() {
    }

    RenderTargetCube::RenderTargetCube(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer, const TextureAttributes& colorTextureAttributes, Vector2u size):
        RenderTarget(hasColor, hasDepth, enableStencilBuffer, colorTextureAttributes, size) {

    }
}