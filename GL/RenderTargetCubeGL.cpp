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
                                       const TextureAttributes& colorTextureAttributes, Vector2u size) :
        RenderTargetCube(hasColor, hasDepth, enableStencilBuffer, colorTextureAttributes, size), RenderTargetGL() {

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
        if (hasDepthBuffer && !enableStencilBuffer) {
            this->depthTexture = Engine::instance()->createTexture2D(this->depthTextureAttributes);
            this->buildAndVerifyTexture(this->depthTexture);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthTexture->getTextureID(), 0);
        }
        else if (hasDepthBuffer && enableStencilBuffer) {
            this->initDepthStencilBufferCombo(this->size.x, this->size.y);
        }

        this->completeFramebuffer();

        return true;
    }

}
