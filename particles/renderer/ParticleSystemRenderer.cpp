#include "ParticleSystemRenderer.h"

namespace Core {

    ParticleSystemRenderer::ParticleSystemRenderer(WeakPointer<Object3D> owner): Object3DRenderer(owner) {
        this->maxParticleCount = maxParticleCount;
    }

    ParticleSystemRenderer::~ParticleSystemRenderer() {

    }
}