#pragma once

#include "../common/gl.h"

namespace Core {

    class RenderTargetGL{
    public:
        RenderTargetGL();
        GLuint getFBOID() const;

    protected:

        // OpenGL Framebuffer Object ID.
        GLuint fboID;

    };
}
