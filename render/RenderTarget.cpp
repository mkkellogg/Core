#include "RenderTarget.h"
#include "../base/BitMask.h"

namespace Core {

    RenderTarget::RenderTarget(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer, const TextureAttributes& colorTextureAttributes, Vector2u size) {
        this->hasColorBuffer = hasColor;
        this->hasDepthBuffer = hasDepth;
        this->enableStencilBuffer = enableStencilBuffer;
        this->size = size;
        this->viewport = Vector4u(0, 0, this->size.x, this->size.y);
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
                return false;
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

    Vector2u RenderTarget::getSize() {
        return this->size;
    }

    Vector4u RenderTarget::getViewport() {
        return this->viewport;
    }

    Bool RenderTarget::isColorBufferTexture() const {
        return colorBufferIsTexture;
    }

    Bool RenderTarget::isDepthBufferTexture() const {
        return depthBufferIsTexture;
    }
}