#pragma once

#include "../util/PersistentWeakPointer.h"
#include "../common/types.h"
#include "../base/BitMask.h"
#include "../geometry/Vector2.h"
#include "../geometry/Vector4.h"
#include "RenderBuffer.h"
#include "../image/TextureAttr.h"
#include "RenderTargetException.h"

namespace Core {

    // forward declarations
    class Texture;

    class RenderTarget {
    public:

        virtual ~RenderTarget();

        virtual Bool init() = 0;
        Bool hasBuffer(RenderBufferType bufferType) const;
        WeakPointer<Texture> getDepthTexture();
        WeakPointer<Texture> getColorTexture();
        Vector2u getSize();
        Vector4u getViewport();
        Bool isColorBufferTexture() const;
        Bool isDepthBufferTexture() const;
        Bool isHDRCapable() const;
        UInt32 getMaxMipLevel() const;
        UInt32 getMipLevel() const;
        void setMipLevel(UInt32 level);
        virtual void destroyColorBuffer() = 0;
        virtual void destroyDepthBuffer() = 0;
        Vector4u getViewportForMipLevel(UInt32 mipLevel);

     protected:

        // does this render target support standard color-buffer rendering?
        Bool hasColorBuffer;
        // does this render target support depth rendering?
        Bool hasDepthBuffer;
        // is the color buffer a texture?
        Bool colorBufferIsTexture;
        // is the depth buffer a texture?
        Bool depthBufferIsTexture;
        // enable stencil buffer for render (but not as a render target)
        Bool enableStencilBuffer;
        // size of this render target
        Vector2u size;
        // viewport for this rendertarget
        Vector4u viewport;
        // texture to which color rendering will occur
        PersistentWeakPointer<Texture> colorTexture;
        // texture to which depth rendering will occur
        PersistentWeakPointer<Texture> depthTexture;
        // texture attributes of [colorTexture]
        TextureAttributes colorTextureAttributes;
        TextureAttributes depthTextureAttributes;

        UInt32 mipLevel;

        RenderTarget(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer, const TextureAttributes& colorTextureAttributes,
                     const TextureAttributes& depthTextureAttributes, Vector2u size);
        Bool buildAndVerifyTexture(WeakPointer<Texture> texture);

    };

}
