#include "../Engine.h"
#include "RenderTargetGL.h"
#include "../common/gl.h"
#include "../base/BitMask.h"
#include "../Graphics.h"
#include "../image/Texture.h"
#include "Texture2DGL.h"

namespace Core {

    RenderTargetGL::RenderTargetGL(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer,
                                   const TextureAttributes& colorTextureAttributes, UInt32 width, UInt32 height) :
        RenderTarget(hasColor, hasDepth, enableStencilBuffer, colorTextureAttributes, width, height) {
        fboID = 0;
    }

    RenderTargetGL::~RenderTargetGL() {
        this->destroy();
    }

    /*
     * Destroy the FBO associated with this render target and all attached textures and/or
     * render buffers.
     */
    void RenderTargetGL::destroy() {

        // destroy the color texture attachment
        if (this->colorTexture.isValid()) {
            Texture2DGL * texGL = dynamic_cast<Texture2DGL*>(colorTexture.get());

        }

        // destroy the depth texture attachment
        if (this->depthTexture.isValid()) {
            Texture2DGL * texGL = dynamic_cast<Texture2DGL*>(depthTexture.get());
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
    Bool RenderTargetGL::init() {
        // make sure to clean up existing frame-buffer objects (if they exist).
        this->destroy();

        // generate an OpenGL FBO.
        glGenFramebuffers(1, &fboID);
        glBindFramebuffer(GL_FRAMEBUFFER, fboID);

        if (fboID == 0) {
            throw RenderTargetException("RenderTargetGL::Init -> Unable to create frame buffer object.");
        }

        // generate a color texture attachment
        /*if (this->hasColorBuffer) {
            TextureAttributes attributes = this->colorTextureAttributes;

            if (attributes.IsCube) {
                colorTexture = Engine::instance()->createCubeTexture(attributes);
                // create a cube texture for this render target
                colorTexture = objectManager->CreateCubeTexture(nullptr, width, height,
                                                                nullptr, width, height,
                                                                nullptr, width, height,
                                                                nullptr, width, height,
                                                                nullptr, width, height,
                                                                nullptr, width, height);
            }
            else {
                // create a 2D texture for this render target
                colorTexture = objectManager->CreateTexture(width, height, nullptr, attributes);
            }
            NONFATAL_ASSERT_RTRN(colorTexture.IsValid(), "RenderTargetGL::Init -> Unable to create color texture.", false, true);

            TextureGL * texGL = dynamic_cast<TextureGL*>(colorTexture.GetPtr());
            ASSERT(texGL != nullptr, "RenderTargetGL::Init -> Unable to cast color texture to TextureGL.");

            if (attributes.IsCube) {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, texGL->GetTextureID(), 0);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, texGL->GetTextureID(), 0);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, texGL->GetTextureID(), 0);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, texGL->GetTextureID(), 0);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, texGL->GetTextureID(), 0);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, texGL->GetTextureID(), 0);

            }
            else
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texGL->GetTextureID(), 0);

            colorBufferIsTexture = true;
        }*/

        // generate a depth texture attachment
       /* if (hasDepthBuffer && !enableStencilBuffer) {
            TextureAttributes attributes;
            attributes.FilterMode = TextureFilter::Point;
            attributes.WrapMode = TextureWrap::Clamp;
            attributes.IsDepthTexture = true;

            depthTexture = objectManager->CreateTexture(width, height, nullptr, attributes);
            NONFATAL_ASSERT_RTRN(depthTexture.IsValid(), "RenderTargetGL::Init -> Unable to create depth texture.", false, true);

            TextureGL * texGL = dynamic_cast<TextureGL*>(depthTexture.GetPtr());
            ASSERT(texGL != nullptr, "RenderTargetGL::Init -> Unable to cast depth texture to TextureGL.");

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texGL->GetTextureID(), 0);

            depthBufferIsTexture = true;
        }
        else if (hasDepthBuffer && enableStencilBuffer) {

            // In OpengL, if we want depth AND stencil abilities then they must both be render buffers and
            // they must be shared.

            GLuint depthStencilRenderBufferID;
            glGenRenderbuffers(1, &depthStencilRenderBufferID);
            ASSERT(depthStencilRenderBufferID != 0, "RenderTargetGL::Init -> Unable to create depth/stencil render buffer.");

            glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRenderBufferID);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

            //Attach stencil buffer to FBO
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilRenderBufferID);

            depthBufferIsTexture = false;
        }

        UInt32 status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        NONFATAL_ASSERT_RTRN(status == GL_FRAMEBUFFER_COMPLETE, "RenderTargetGL::Init -> Framebuffer is incomplete!.", false, true);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

        return true;
    }

    /*
     * Get the OpenGL FBO ID.
     */
    GLuint RenderTargetGL::getFBOID() const {
        return fboID;
    }
}
