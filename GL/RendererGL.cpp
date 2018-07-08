#include "RendererGL.h"
#include "../geometry/Mesh.h"

namespace Core {

    RendererGL::RendererGL() {
    }

    Bool RendererGL::init() {
        Renderer::init();
        return true;
    }

    RendererGL::~RendererGL() {
    }

}
