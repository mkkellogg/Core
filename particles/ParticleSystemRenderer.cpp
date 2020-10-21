#include "ParticleSystemRenderer.h"

namespace Core {

    ParticleSystemRenderer::ParticleSystemRenderer(WeakPointer<Graphics> graphics, WeakPointer<Object3D> owner): Object3DRenderer(graphics, owner) {
        this->maxParticleCount = maxParticleCount;
    }

}