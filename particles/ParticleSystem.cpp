#include "ParticleSystem.h"

namespace Core {

    ParticleSystem::ParticleSystem(WeakPointer<Object3D> owner, UInt32 maximumActiveParticles): Object3DComponent(owner) {
        this->maximumActiveParticles = maximumActiveParticles;
    }

    ParticleSystem::~ParticleSystem() {
    }

    WeakPointer<ParticleEmitter> ParticleSystem::getEmitter() {
        return this->particleEmitter;
    }

    UInt32 ParticleSystem::getMaximumActiveParticles() {
        return this->maximumActiveParticles;
    }
}
