#include "BaseObjectRenderer.h"

namespace Core {

    Bool BaseObjectRenderer::forwardRender(const ViewDescriptor& viewDescriptor, const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        return false;
    }

    Bool BaseObjectRenderer::forwardRenderObject(const ViewDescriptor& viewDescriptor, WeakPointer<BaseRenderable> renderable, Bool isStatic,
                                                 const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
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
