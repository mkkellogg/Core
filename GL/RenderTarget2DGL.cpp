#include "../Engine.h"
#include "RenderTarget2DGL.h"
#include "../common/gl.h"
#include "../base/BitMask.h"
#include "../Graphics.h"
#include "../image/Texture.h"
#include "Texture2DGL.h"
#include "CubeTextureGL.h"
#include "GraphicsGL.h"

namespace Core {

    RenderTarget2DGL::RenderTarget2DGL(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer,
                                       const TextureAttributes& colorTextureAttributes, 
                                       const TextureAttributes& depthTextureAttributes, Vector2u size,
                                       Int32 initialFBOID) :
        RenderTarget2D(hasColor, hasDepth, enableStencilBuffer, colorTextureAttributes, depthTextureAttributes, size), RenderTargetGL(initialFBOID) {

    }

    RenderTarget2DGL::~RenderTarget2DGL() {
        for (UInt32 i = 0; i < this->activeColorTextures; i++) {
            this->destroyColorBuffer(i);
        }
        this->destroyDepthBuffer();
    }

    /*
     * Perform all initialization for this render target. This render target will not
     * be valid until this method successfully completes.
     */
    Bool RenderTarget2DGL::init() {

        this->initFramebuffer();

        // generate a color texture attachment
        // TODO: For now we are only supporting a texture type color attachment
        if (this->hasColorBuffer) {
            this->initColorTexture(0);
        }

        // generate a depth texture attachment
        // TODO: For now we are only supporting a texture type depth attachment if a stencil attachment is not included
        if (this->hasDepthBuffer && !this->enableStencilBuffer) {
            this->depthTexture = Engine::instance()->createTexture2D(this->depthTextureAttributes);
            this->buildAndVerifyTexture(this->depthTexture);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthTexture->getTextureID(), 0);
        }
        else if (this->hasDepthBuffer && this->enableStencilBuffer) {
            this->initDepthStencilBufferCombo(this->size.x, this->size.y);
        }

        this->completeFramebuffer();
        return true;
    }

    Bool RenderTarget2DGL::addColorTexture(TextureAttributes attributes) {
        if (this->activeColorTextures < RenderTarget::MaxRenderTargetOutputTargets - 1) {
            glBindFramebuffer(GL_FRAMEBUFFER, this->fboID);
            this->colorTextureAttributes[this->activeColorTextures] = attributes;
            this->mipLevel[this->activeColorTextures] = 0;
            this->colorBufferIsTexture[this->activeColorTextures] = true;
            this->initColorTexture(this->activeColorTextures);
            this->activeColorTextures++;
            this->completeFramebuffer();
        }
        return true;
    }

    Bool RenderTarget2DGL::initColorTexture(UInt32 index) {
        this->colorTexture[index] = Engine::instance()->createTexture2D(this->colorTextureAttributes[index]);
        this->buildAndVerifyTexture(this->colorTexture[index]);
        GLint attachment = GraphicsGL::getColorAttachmentID(index);
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, this->colorTexture[index]->getTextureID(), 0);
        return true;
    }

    void RenderTarget2DGL::destroyColorBuffer(UInt32 index) {
        if (index > this->activeColorTextures) {
            throw OutOfRangeException("RenderTarget2DGL::destroyColorBuffer -> Output color target index is out of range.");
        }
        if (this->hasColorBuffer) {
            if (this->colorTexture[index]) {
                WeakPointer<Texture2D> texture = WeakPointer<Texture>::dynamicPointerCast<Texture2D>(this->colorTexture[index]);
                Graphics::safeReleaseObject(texture);
                this->colorTexture[index] = WeakPointer<Texture>::nullPtr();
            }
        }
    }

    void RenderTarget2DGL::destroyDepthBuffer() {
        if (this->hasDepthBuffer) {
            if (!this->enableStencilBuffer) {
                if (this->depthTexture) {
                    WeakPointer<Texture2D> texture = WeakPointer<Texture>::dynamicPointerCast<Texture2D>(this->depthTexture);
                    Graphics::safeReleaseObject(texture);
                    this->depthTexture = WeakPointer<Texture>::nullPtr();
                }
            }
            else if (this->hasDepthBuffer && this->enableStencilBuffer) {
                this->destroyDepthStencilBufferCombo();
            }
        }
    }

}
