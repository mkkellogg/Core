#pragma once

#include "../common/gl.h"
#include "../common/types.h"
#include "../render/RenderTargetException.h"

namespace Core {

    class RenderTargetGL{
    public:
        RenderTargetGL(Int32 initialFBOID = 0);
        ~RenderTargetGL();
        GLuint getFBOID() const;
        void setFBOID(GLuint id);

    protected:
        void destroy();
        void initFramebuffer();
        void completeFramebuffer();
        void initDepthStencilBufferCombo(UInt32 sizeX, UInt32 sizeY);

        // OpenGL Framebuffer Object ID.
        GLuint fboID;

    };
}
