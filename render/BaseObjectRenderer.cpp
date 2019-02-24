#include "BaseObjectRenderer.h"

namespace Core {

    Bool BaseObjectRenderer::forwardRender(const ViewDescriptor& viewDescriptor, const std::vector<WeakPointer<Light>>& lights) {
        return false;
    }

    Bool BaseObjectRenderer::supportsRenderPath(RenderPath renderPath) {
        return false;
    }

    Bool BaseObjectRenderer::castsShadows() {
        return this->castShadows;
    }

    void BaseObjectRenderer::setCastShadows(Bool castShadows) {
        this->castShadows = castShadows;
    }
}
