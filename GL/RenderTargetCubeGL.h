/*
 * class:  RenderTargetCubeGL
 *
 * Author: Mark Kellogg
 *
 * OpenGL implementation of RenderTargetCube. Currently it makes use of Framebuffer Objects (FBO)
 * with a single texture color attachment at GL_COLOR_ATTACHMENT0, and single texture
 * depth attachment at GL_DEPTH_ATTACHMENT (as of now only one depth attachment is supported ).
 */

#pragma once

#include "../common/gl.h"
#include "../render/RenderTargetCube.h"
#include "../base/BitMask.h"
#include "../common/Exception.h"
#include "RenderTargetGL.h"

namespace Core {

    // forward declarations
    class TextureAttributes;
    class GraphicsGL;

    class RenderTargetCubeGL final : public RenderTargetCube, public RenderTargetGL {
        friend class GraphicsGL;
    public:
        
        ~RenderTargetCubeGL();
        Bool init() override;

    private:

        RenderTargetCubeGL(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer,
                           const TextureAttributes& colorTextureAttributes, 
                           const TextureAttributes& depthTextureAttributes, Vector2u size);
    };
}
