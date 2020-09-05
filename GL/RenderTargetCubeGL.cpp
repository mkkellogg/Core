#include "../Engine.h"
#include "RenderTargetCubeGL.h"
#include "../common/gl.h"
#include "../base/BitMask.h"
#include "../Graphics.h"
#include "../image/Texture.h"
#include "Texture2DGL.h"
#include "CubeTextureGL.h"

namespace Core {

    RenderTargetCubeGL::RenderTargetCubeGL(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer,
                                       const TextureAttributes& colorTextureAttributes,
                                       const TextureAttributes& depthTextureAttributes, Vector2u size) :
        RenderTargetCube(hasColor, hasDepth, enableStencilBuffer, colorTextureAttributes, depthTextureAttributes, size), RenderTargetGL() {

    }

    RenderTargetCubeGL::~RenderTargetCubeGL() {
        for (UInt32 i = 0; i < this->activeColorTextures; i++) {
            this->destroyColorBuffer(i);
        }
        this->destroyDepthBuffer();
    }

    /*
     * Perform all initialization for this render target. This render target will not
     * be valid until this method successfully completes.
     */
    Bool RenderTargetCubeGL::init() {
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

    Bool RenderTargetCubeGL::addColorTexture(TextureAttributes attributes) {
        Bool success = true;
        if (this->activeColorTextures < RenderTarget::MaxRenderTargetOutputTargets - 1) {
            this->colorTextureAttributes[this->activeColorTextures] = attributes;
            this->mipLevel[this->activeColorTextures] = 0;
            this->colorBufferIsTexture[this->activeColorTextures] = true;
            success = success && this->initColorTexture(this->activeColorTextures);
            this->activeColorTextures++;
        }
        return success;
    }

    Bool RenderTargetCubeGL::initColorTexture(UInt32 index) {
        this->colorTexture[index] = Engine::instance()->createCubeTexture(this->colorTextureAttributes[index]);
        this->buildAndVerifyTexture(this->colorTexture[index]);

        GLuint colorTexID = this->colorTexture[index]->getTextureID();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, colorTexID, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, colorTexID, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, colorTexID, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, colorTexID, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, colorTexID, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, colorTexID, 0);

        return true;
    }

    void RenderTargetCubeGL::destroyColorBuffer(UInt32 index) {
        if (index > this->activeColorTextures) {
            throw OutOfRangeException("RenderTargetCubeGL::destroyColorBuffer -> Output color target index is out of range.");
        }
        if (this->hasColorBuffer) {
            if (this->colorTexture[index]) {
                WeakPointer<CubeTexture> texture = WeakPointer<Texture>::dynamicPointerCast<CubeTexture>(this->colorTexture[index]);
                Graphics::safeReleaseObject(texture);
                this->colorTexture[index] = WeakPointer<Texture>::nullPtr();
            }
        }
    }

    void RenderTargetCubeGL::destroyDepthBuffer() {
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
