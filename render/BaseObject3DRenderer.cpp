#include "BaseObject3DRenderer.h"

namespace Core {

    Bool BaseObject3DRenderer::forwardRender(const ViewDescriptor& viewDescriptor, const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        return false;
    }

    Bool BaseObject3DRenderer::forwardRenderObject(const ViewDescriptor& viewDescriptor, WeakPointer<BaseRenderable> renderable, Bool isStatic,
                                                   const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        return false;
    }

    Bool BaseObject3DRenderer::supportsRenderPath(RenderPath renderPath) {
        return false;
    }

    Bool BaseObject3DRenderer::castsShadows() {
        return this->castShadows;
    }

    void BaseObject3DRenderer::setCastShadows(Bool castShadows) {
        this->castShadows = castShadows;
    }
}
