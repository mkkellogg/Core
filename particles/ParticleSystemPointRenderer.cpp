#include "ParticleSystemPointRenderer.h"
#include "ParticleSystem.h"

namespace Core {

    ParticleSystemPointRenderer::ParticleSystemPointRenderer(WeakPointer<Graphics> graphics, WeakPointer<Object3D> owner):
        ParticleSystemRenderer(graphics, owner) {}

    ParticleSystemPointRenderer::~ParticleSystemPointRenderer() {}

    void ParticleSystemPointRenderer::init() {

    }

    Bool ParticleSystemPointRenderer::forwardRender(const ViewDescriptor& viewDescriptor, const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {

    }

    Bool ParticleSystemPointRenderer::forwardRenderObject(const ViewDescriptor& viewDescriptor, WeakPointer<BaseRenderable> renderable, Bool isStatic,
                                                                   const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {

    }

    Bool ParticleSystemPointRenderer::forwardRenderParticleSystem(const ViewDescriptor& viewDescriptor, WeakPointer<ParticleSystem> particleSystem, Bool isStatic,
                                                                           const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        UInt32 maxActiveParticles = particleSystem->getMaximumActiveParticles();
        if (maxActiveParticles != this->renderAttributes.getParticleCount()) {
            this->renderAttributes.setParticleCount(maxActiveParticles);
        }
    }

    Bool ParticleSystemPointRenderer::supportsRenderPath(RenderPath renderPath) {

    }

    UInt32 ParticleSystemPointRenderer::getRenderQueueID() const {

    }

}
