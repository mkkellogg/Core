#include "ParticleSystemAnimatedSpriteRenderer.h"
#include "../ParticleSystem.h"

namespace Core {

    ParticleSystemAnimatedSpriteRenderer::ParticleSystemAnimatedSpriteRenderer(WeakPointer<Object3D> owner):
        ParticleSystemRenderer(owner) {}

    ParticleSystemAnimatedSpriteRenderer::~ParticleSystemAnimatedSpriteRenderer() {}

    void ParticleSystemAnimatedSpriteRenderer::init() {

    }

    Bool ParticleSystemAnimatedSpriteRenderer::forwardRender(const ViewDescriptor& viewDescriptor, const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {

    }

    Bool ParticleSystemAnimatedSpriteRenderer::forwardRenderObject(const ViewDescriptor& viewDescriptor, WeakPointer<BaseRenderable> renderable, Bool isStatic,
                                                                   const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {

    }

    Bool ParticleSystemAnimatedSpriteRenderer::forwardRenderParticleSystem(const ViewDescriptor& viewDescriptor, WeakPointer<ParticleSystem> particleSystem, Bool isStatic,
                                                                           const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        UInt32 maxActiveParticles = particleSystem->getMaximumActiveParticles();
        if (maxActiveParticles != this->renderAttributes.getParticleCount()) {
            this->renderAttributes.setParticleCount(maxActiveParticles);
        }
    }

    Bool ParticleSystemAnimatedSpriteRenderer::supportsRenderPath(RenderPath renderPath) {

    }

    UInt32 ParticleSystemAnimatedSpriteRenderer::getRenderQueueID() const {

    }

}
