#pragma once

#include "../util/PersistentWeakPointer.h"
#include "../common/types.h"
#include "../base/BitMask.h"
#include "RenderBuffer.h"
#include "../image/TextureAttr.h"

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
        UInt32 getWidth() const;
        UInt32 getHeight() const;
        Bool isColorBufferTexture() const;
        Bool isDepthBufferTexture() const;

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
        // width of this render target
        UInt32 width;
        // height of this render target
        UInt32 height;
        // texture to which color rendering will occur
        PersistentWeakPointer<Texture> colorTexture;
        // texture to which depth rendering will occur
        PersistentWeakPointer<Texture> depthTexture;
        // texture attributes of [colorTexture]
        TextureAttributes colorTextureAttributes;

        RenderTarget(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer, const TextureAttributes& colorTextureAttributes, UInt32 width, UInt32 height);


    };

}
