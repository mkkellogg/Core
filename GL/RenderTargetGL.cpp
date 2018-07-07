#include "RenderTargetGL.h"

namespace Core {

    RenderTargetGL::RenderTargetGL(): fboID(0) {

    }

    /*
     * Get the OpenGL FBO ID.
     */
    GLuint RenderTargetGL::getFBOID() const {
        return fboID;
    }
}
