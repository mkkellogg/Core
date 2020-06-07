#pragma once

#include "../util/PersistentWeakPointer.h"
#include "../common/types.h"
#include "../base/CoreObject.h"
#include "../base/BitMask.h"
#include "../geometry/Vector2.h"
#include "../geometry/Vector4.h"
#include "RenderBuffer.h"
#include "../image/TextureAttr.h"
#include "RenderTargetException.h"

namespace Core {

    // forward declarations
    class Texture;

    class RenderTarget : public CoreObject {
    public:

        static const UInt32 MaxRenderTargetOutputTargets = 3;

        virtual ~RenderTarget();

        virtual Bool init() = 0;
        Bool hasBuffer(RenderBufferType bufferType) const;
        WeakPointer<Texture> getDepthTexture();
        WeakPointer<Texture> getColorTexture(UInt32 index = 0);
        virtual Bool addColorTexture(TextureAttributes) = 0;
        Vector2u getSize();
        Vector4u getViewport();
        Bool isColorBufferTexture(UInt32 index = 0) const;
        Bool isDepthBufferTexture() const;
        Bool isHDRCapable() const;
        UInt32 getMaxMipLevel(UInt32 index = 0) const;
        UInt32 getMipLevel(UInt32 index = 0) const;
        void setMipLevel(UInt32 level, UInt32 index = 0);
        virtual void destroyColorBuffer(UInt32 index = 0) = 0;
        virtual void destroyDepthBuffer() = 0;
        Vector4u getViewportForMipLevel(UInt32 mipLevel);

     protected:

        // does this render target support standard color-buffer rendering?
        Bool hasColorBuffer;
        // does this render target support depth rendering?
        Bool hasDepthBuffer;
        // is the color buffer a texture?
        Bool colorBufferIsTexture[MaxRenderTargetOutputTargets];
        // is the depth buffer a texture?
        Bool depthBufferIsTexture;
        // enable stencil buffer for render (but not as a render target)
        Bool enableStencilBuffer;
        // size of this render target
        Vector2u size;
        // viewport for this rendertarget
        Vector4u viewport;
        // texture to which color rendering will occur
        PersistentWeakPointer<Texture> colorTexture[MaxRenderTargetOutputTargets];
        // number of activated output color textures
        UInt32 activeColorTextures;
        // texture to which depth rendering will occur
        PersistentWeakPointer<Texture> depthTexture;
        // texture attributes of [colorTexture]
        TextureAttributes colorTextureAttributes[MaxRenderTargetOutputTargets];
        TextureAttributes depthTextureAttributes;

        UInt32 mipLevel[MaxRenderTargetOutputTargets];

        RenderTarget(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer, const TextureAttributes& colorTextureAttributes,
                     const TextureAttributes& depthTextureAttributes, Vector2u size);
        Bool buildAndVerifyTexture(WeakPointer<Texture> texture);

    };

}
