#include "ParticleSystemManager.h"
#include "ParticleSystem.h"
#include "../util/Time.h"

namespace Core {

    ParticleSystemManager::ParticleSystemManager() {
    }

    ParticleSystemManager::~ParticleSystemManager() {
    }

    void ParticleSystemManager::update() {
        for(WeakPointer<ParticleSystem> particleSystem : this->particleSystems) {
            particleSystem->update(Time::getDeltaTime());
        }
    }

    void ParticleSystemManager::addParticleSystem(WeakPointer<ParticleSystem> particleSystem) {
        if (!particleSystem.isValid()) {
            throw InvalidArgumentException("ParticleSystemManager::addParticleSystem() -> 'particleSystem' is invalid."); 
        }
        this->particleSystems.push_back(particleSystem);
    }
}
