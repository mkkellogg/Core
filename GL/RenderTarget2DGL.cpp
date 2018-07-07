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
        this->destroy();
    }

    /*
     * Destroy the FBO associated with this render target and all attached textures and/or
     * render buffers.
     */
    void RenderTarget2DGL::destroy() {

        // destroy the color texture attachment
        if (this->colorTexture.isValid()) {
            GLuint texID = this->colorTexture->getTextureID();
            glDeleteTextures(1, &texID);
        }

        // destroy the depth texture attachment
        if (this->depthTexture.isValid()) {
            GLuint texID = this->depthTexture->getTextureID();
            glDeleteTextures(1, &texID);
        }

        // destroy the FBO
        if (fboID > 0) {
            glDeleteFramebuffers(1, &fboID);
        }
    }

    /*
     * Perform all initialization for this render target. This render target will not
     * be valid until this method successfully completes.
     */
    Bool RenderTarget2DGL::init() {
        // make sure to clean up existing frame-buffer objects (if they exist).
        this->destroy();

        // generate an OpenGL FBO.
        glGenFramebuffers(1, &fboID);
        glBindFramebuffer(GL_FRAMEBUFFER, fboID);

        if (fboID == 0) {
            throw RenderTargetException("RenderTarget2DGL::init -> Unable to create frame buffer object.");
        }

        // generate a color texture attachment
        if (this->hasColorBuffer) {
            TextureAttributes attributes = this->colorTextureAttributes;

            if (attributes.IsCube) {
                colorTexture = Engine::instance()->createCubeTexture(attributes);
            }
            else {
                colorTexture = Engine::instance()->createTexture2D(attributes);
            }
            colorTexture->build(this->size.x, this->size.y);
            if (!colorTexture.isValid()) {
                throw RenderTargetException("RenderTarget2DGL::init -> Unable to create color texture.");
            }

            if (attributes.IsCube) {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, this->colorTexture->getTextureID(), 0);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, this->colorTexture->getTextureID(), 0);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, this->colorTexture->getTextureID(), 0);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, this->colorTexture->getTextureID(), 0);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, this->colorTexture->getTextureID(), 0);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, this->colorTexture->getTextureID(), 0);

            }
            else {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->colorTexture->getTextureID(), 0);
            }

            colorBufferIsTexture = true;
        }

        // generate a depth texture attachment
        if (hasDepthBuffer && !enableStencilBuffer) {
            TextureAttributes attributes;
            attributes.FilterMode = TextureFilter::Point;
            attributes.WrapMode = TextureWrap::Clamp;
            attributes.IsDepthTexture = true;

            this->depthTexture = Engine::instance()->createTexture2D(attributes);
            if (!this->depthTexture.isValid()) {
                throw RenderTargetException("RenderTarget2DGL::init -> Unable to create depth texture.");
            }
            this->depthTexture->build(this->size.x, this->size.y);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthTexture->getTextureID(), 0);

            depthBufferIsTexture = true;
        }
        else if (hasDepthBuffer && enableStencilBuffer) {

            // In OpengL, if we want depth AND stencil abilities then they must both be render buffers and
            // they must be shared.

            GLuint depthStencilRenderBufferID;
            glGenRenderbuffers(1, &depthStencilRenderBufferID);

            if (depthStencilRenderBufferID == 0) {
                throw RenderTargetException("RenderTarget2DGL::init -> Unable to create depth/stencil render buffer.");
            }

            glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRenderBufferID);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->size.x, this->size.y);

            //Attach stencil buffer to FBO
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilRenderBufferID);

            depthBufferIsTexture = false;
        }

        UInt32 status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if (status != GL_FRAMEBUFFER_COMPLETE) {
            throw RenderTargetException("RenderTarget2DGL::init -> Framebuffer is incomplete!.");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return true;
    }

}
