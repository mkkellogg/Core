#include "RenderTargetCube.h"

namespace Core {

    RenderTargetCube::~RenderTargetCube() {
    }

    RenderTargetCube::RenderTargetCube(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer, const TextureAttributes& colorTextureAttributes,
                                       const TextureAttributes& depthTextureAttributes, Vector2u size):
        RenderTarget(hasColor, hasDepth, enableStencilBuffer, colorTextureAttributes, depthTextureAttributes, size, true) {

    }
}