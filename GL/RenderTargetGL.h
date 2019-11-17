#pragma once

#include "../common/gl.h"
#include "../common/types.h"
#include "../render/RenderTargetException.h"

namespace Core {

    class RenderTargetGL{
    public:
        RenderTargetGL(Int32 initialFBOID = 0);
        virtual ~RenderTargetGL();
        GLuint getFBOID() const;
        void setFBOID(GLuint id);

    protected:
        void destroyFrameBuffer();
        void initFramebuffer();
        void completeFramebuffer();
        void initDepthStencilBufferCombo(UInt32 sizeX, UInt32 sizeY);
        void destroyDepthStencilBufferCombo();

        // OpenGL Framebuffer Object ID.
        GLuint fboID;
        GLuint depthStencilRenderBufferID;
    };
}
