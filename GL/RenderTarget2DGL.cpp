#include "../Engine.h"
#include "RenderTarget2DGL.h"
#include "../common/gl.h"
#include "../base/BitMask.h"
#include "../Graphics.h"
#include "../image/Texture.h"
#include "Texture2DGL.h"
#include "CubeTextureGL.h"

namespace Core {

    RenderTarget2DGL::RenderTarget2DGL(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer,
                                       const TextureAttributes& colorTextureAttributes, Vector2u size) :
        RenderTarget2D(hasColor, hasDepth, enableStencilBuffer, colorTextureAttributes, size), RenderTargetGL() {

    }

    RenderTarget2DGL::~RenderTarget2DGL() {
       
    }

    /*
     * Perform all initialization for this render target. This render target will not
     * be valid until this method successfully completes.
     */
    Bool RenderTarget2DGL::init() {

        this->initFramebuffer();

        // generate a color texture attachment
        if (this->hasColorBuffer) {
            colorTexture = Engine::instance()->createTexture2D(this->colorTextureAttributes);
            colorTexture->build(this->size.x, this->size.y);
            if (!colorTexture.isValid()) {
                throw RenderTargetException("RenderTarget2DGL::init -> Unable to create color texture.");
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->colorTexture->getTextureID(), 0);
        }

        // generate a depth texture attachment
        if (hasDepthBuffer && !enableStencilBuffer) {
            this->depthTexture = Engine::instance()->createTexture2D(this->depthTextureAttributes);
            if (!this->depthTexture.isValid()) {
                throw RenderTargetException("RenderTarget2DGL::init -> Unable to create depth texture.");
            }
            this->depthTexture->build(this->size.x, this->size.y);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthTexture->getTextureID(), 0);
        }
        else if (hasDepthBuffer && enableStencilBuffer) {
            this->initDepthStencilBufferCombo(this->size.x, this->size.y);
        }

        this->completeFramebuffer();
        return true;
    }

}
