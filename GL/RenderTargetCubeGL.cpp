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
            this->colorTexture = Engine::instance()->createCubeTexture(this->colorTextureAttributes);
            this->buildAndVerifyTexture(this->colorTexture);

            GLuint colorTexID = this->colorTexture->getTextureID();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, colorTexID, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, colorTexID, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, colorTexID, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, colorTexID, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, colorTexID, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, colorTexID, 0);
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

    void RenderTargetCubeGL::destroyColorBuffer() {
        if (this->hasColorBuffer) {
            if (this->colorTexture) {
                WeakPointer<CubeTexture> texture = WeakPointer<Texture>::dynamicPointerCast<CubeTexture>(this->colorTexture);
                Engine::instance()->destroyCubeTexture(texture);
                this->colorTexture = WeakPointer<Texture>::nullPtr();
            }
        }
    }

    void RenderTargetCubeGL::destroyDepthBuffer() {
        if (this->hasDepthBuffer) {
            if (!this->enableStencilBuffer) {
                if (this->depthTexture) {
                    WeakPointer<Texture2D> texture = WeakPointer<Texture>::dynamicPointerCast<Texture2D>(this->depthTexture);
                    Engine::instance()->destroyTexture2D(texture);
                    this->depthTexture = WeakPointer<Texture>::nullPtr();
                }
            }
            else if (this->hasDepthBuffer && this->enableStencilBuffer) {
                this->destroyDepthStencilBufferCombo();
            }
        }
    }
}
