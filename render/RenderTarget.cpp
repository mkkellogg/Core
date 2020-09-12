#include "RenderTarget.h"
#include "../base/BitMask.h"
#include "../image/Texture.h"
#include "../math/Math.h"

namespace Core {

    RenderTarget::RenderTarget(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer, const TextureAttributes& colorTextureAttributes, 
                               const TextureAttributes& depthTextureAttributes, Vector2u size, Bool cube) {
        this->cube = cube;
        this->activeColorTextures = 0;
        this->hasColorBuffer = hasColor;
        this->hasDepthBuffer = hasDepth;
        this->enableStencilBuffer = enableStencilBuffer;
        this->size = size;
        this->viewport = Vector4u(0, 0, this->size.x, this->size.y);
        this->colorTextureAttributes[0] = colorTextureAttributes;

        this->depthTextureAttributes = depthTextureAttributes;
        this->depthTextureAttributes.WrapMode = TextureWrap::Clamp;
        this->depthTextureAttributes.IsDepthTexture = true;

        this->mipLevel[0] = 0;

        this->depthBufferIsTexture = false;
        this->colorBufferIsTexture[0] = false;

        // TODO: For now we are only supporting textures for the color attachment
        if (this->hasColorBuffer) {
            this->activeColorTextures = 1;
            colorBufferIsTexture[0] = true;
        }

        // TODO: For now we are only supporting textures for the depth attachment if a stencil buffer is not included
        if (this->hasDepthBuffer && !this->enableStencilBuffer) {
            depthBufferIsTexture = true;
        }
        else if (this->hasDepthBuffer && this->enableStencilBuffer) {
            depthBufferIsTexture = false;
        }

    }

    RenderTarget::~RenderTarget() {

    }

    Bool RenderTarget::isCube() const {
        return this->cube;
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

    UInt32 RenderTarget::getColorTextureCount() const {
        return this->activeColorTextures;
    }

    /*
     * Get a reference to the color texture.
     */
    WeakPointer<Texture> RenderTarget::getColorTexture(UInt32 index) {
        if (index >= this->activeColorTextures) {
            throw OutOfRangeException("RenderTarget::getColorTexture -> Output color target index is out of range.");
        }
        return colorTexture[index];
    }

    Vector2u RenderTarget::getSize() {
        return this->size;
    }

    Vector4u RenderTarget::getViewport() {
        return this->viewport;
    }

    Bool RenderTarget::isColorBufferTexture(UInt32 index) const {
        if (index > this->activeColorTextures) {
            throw OutOfRangeException("RenderTarget::isColorBufferTexture -> Output color target index is out of range.");
        }
        return colorBufferIsTexture[index];
    }

    Bool RenderTarget::isDepthBufferTexture() const {
        return depthBufferIsTexture;
    }

    Bool RenderTarget::isHDRCapable() const {
        Bool capable = true;
        for (UInt32 i = 0; i < this->activeColorTextures; i++) {
            capable = capable && this->colorTextureAttributes[i].Format == TextureFormat::RGBA16F ||
                                 this->colorTextureAttributes[i].Format == TextureFormat::RGBA32F;
        }
        return capable;
    }

    UInt32 RenderTarget::getMaxMipLevel(UInt32 index) const {
        if (index > this->activeColorTextures) {
            throw OutOfRangeException("RenderTarget::getMaxMipLevel -> Output color target index is out of range.");
        }
        return Math::max(this->colorTextureAttributes[index].MipLevels - 1, 0u);
    }

    UInt32 RenderTarget::getMipLevel(UInt32 index) const {
        if (index > this->activeColorTextures) {
            throw OutOfRangeException("RenderTarget::getMipLevel -> Output color target index is out of range.");
        }
        return this->mipLevel[index];
    }

    void RenderTarget::setMipLevel(UInt32 level, UInt32 index) {
        if (index > this->activeColorTextures) {
            throw OutOfRangeException("RenderTarget::setMipLevel -> Output color target index is out of range.");
        }
        this->mipLevel[index] = level;
    }

    Vector4u RenderTarget::getViewportForMipLevel(UInt32 mipLevel) {
        Real mipDimModifier = 1.0f / (Real)(1 << mipLevel);
       
        Vector4u mipLevelScaledViewport;
        mipLevelScaledViewport.x = (UInt32)((Real)this->viewport.x * mipDimModifier);
        mipLevelScaledViewport.y = (UInt32)((Real)this->viewport.y * mipDimModifier);
        mipLevelScaledViewport.z = (UInt32)((Real)this->viewport.z * mipDimModifier);
        mipLevelScaledViewport.w = (UInt32)((Real)this->viewport.w * mipDimModifier);

        return mipLevelScaledViewport;

    }

    Bool RenderTarget::buildAndVerifyTexture(WeakPointer<Texture> texture) {
        if (!texture.isValid()) {
            throw RenderTargetException("RenderTarget::buildAndVerifyTexture -> Texture is not valid.");
        }
        texture->buildEmpty(this->size.x, this->size.y);
        return true;
    }
}