/*
 * class:  RenderTargetGL
 *
 * Author: Mark Kellogg
 *
 * OpenGL implementation of RenderTarget. Currently it makes use of Framebuffer Objects (FBO)
 * with a single texture color attachment at GL_COLOR_ATTACHMENT0, and single texture
 * depth attachment at GL_DEPTH_ATTACHMENT (as of now only one depth attachment is supported ).
 */

#pragma once

#include "../common/gl.h"
#include "../render/RenderTarget.h"
#include "../base/BitMask.h"
#include "../common/Exception.h"

namespace Core {

    // forward declarations
    class TextureAttributes;
    class GraphicsGL;

    class RenderTargetGL final : public RenderTarget {
        friend class GraphicsGL;
    public:

        class RenderTargetException: Exception {
        public:
            RenderTargetException(const std::string& msg): Exception(msg) {}
            RenderTargetException(const char* msg): Exception(msg) {}
        };
        
        ~RenderTargetGL();
        Bool init();
        GLuint getFBOID() const;

    private:

        // OpenGL Framebuffer Object ID.
        GLuint fboID;

        RenderTargetGL(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer,
                       const TextureAttributes& colorTextureAttributes, Vector2u size);

        void destroy();
    };
}
