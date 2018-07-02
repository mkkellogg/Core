#include "RenderTarget.h"
#include "../base/BitMask.h"

namespace Core {

    RenderTarget::RenderTarget(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer, const TextureAttributes& colorTextureAttributes, UInt32 width, UInt32 height) {
        this->hasColorBuffer = hasColor;
        this->hasDepthBuffer = hasDepth;
        this->enableStencilBuffer = enableStencilBuffer;
        this->width = width;
        this->height = height;
        this->colorTextureAttributes = colorTextureAttributes;

        depthBufferIsTexture = false;
        colorBufferIsTexture = false;
    }

    RenderTarget::~RenderTarget() {

    }

    /*
     * Return true if this render target supports [bufferType].
     */
    Bool RenderTarget::hasBuffer(RenderBufferType bufferType) const {
        switch (bufferType) {
            case RenderBufferType::Color:
                return hasColorBuffer;
            case RenderBufferType::Depth:
                return hasDepthBuffer;
            default:
                return false;;
        }
    }

    /*
     * Get a reference to the depth texture.
     */
    WeakPointer<Texture> RenderTarget::getDepthTexture() {
        return depthTexture;
    }

    /*
     * Get a reference to the color texture.
     */
    WeakPointer<Texture> RenderTarget::getColorTexture() {
        return colorTexture;
    }

    UInt32 RenderTarget::getWidth() const {
        return width;
    }

    UInt32 RenderTarget::getHeight() const {
        return height;
    }

    Bool RenderTarget::isColorBufferTexture() const {
        return colorBufferIsTexture;
    }

    Bool RenderTarget::isDepthBufferTexture() const {
        return depthBufferIsTexture;
    }
}