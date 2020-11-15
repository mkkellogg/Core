#include "ParticleSystem.h"

namespace Core {

    ParticleSystem::ParticleSystem(WeakPointer<Object3D> owner, UInt32 maximumActiveParticles): Object3DComponent(owner) {
        this->systemState = SystemState::NotStarted;
        this->maximumActiveParticles = maximumActiveParticles;
        this->activeParticleCount = 0;
        this->emitterInitialized = false;

        this->particleStates.setParticleCount(maximumActiveParticles);
    }

    ParticleSystem::~ParticleSystem() {
    }

    void ParticleSystem::update(Real timeDelta) {
        if (this->emitterInitialized && this->systemState == SystemState::Running) {
            UInt32 particlesToEmit = this->particleEmitter->update(timeDelta);
            if (particlesToEmit > 0) this->activateParticles(particlesToEmit);
            this->advanceActiveParticles(timeDelta);
        }
    }

    void ParticleSystem::start() {
        if (this->systemState == SystemState::NotStarted || this->systemState == SystemState::Paused) {
            this->systemState = SystemState::Running;
        } else {
            // TODO: Decide how to handle this case
        }
    }

    void ParticleSystem::pause() {
        if (this->systemState == SystemState::Running) {
            this->systemState = SystemState::Paused;
        }
    }

    void ParticleSystem::stop() {

    }

    void ParticleSystem::activateParticles(UInt32 particleCount) {
        UInt32 newActiveParticleCount = Math::clamp(this->activeParticleCount + particleCount, 0, this->maximumActiveParticles);
        for (UInt32 i = this->activeParticleCount; i < newActiveParticleCount; i++) {
            this->activateParticle(i);
        }
        this->activeParticleCount = newActiveParticleCount;
    }

    void ParticleSystem::activateParticle(UInt32 index) {
        ParticleStatePtr statePtr = this->particleStates.getParticleStatePtr(index);
        for (UInt32 i = 0; i < this->particleStateInitializers.size(); i++) {
            std::shared_ptr<ParticleStateInitializer> particleStateInitializer = this->particleStateInitializers[i];
            particleStateInitializer->initializeState(statePtr);
        }
    }

    void ParticleSystem::advanceActiveParticles(Real timeDelta) {
        UInt32 i = 0;
        while (i < this->activeParticleCount) {
            Bool particleIsActive = this->advanceActiveParticle(i, timeDelta);
            if (!particleIsActive) {
                if (i < this->activeParticleCount - 1) {
                    this->copyParticleInArray(this->activeParticleCount - 1, i);
                }
                this->activeParticleCount--;
                continue;
            }
            i++;
        }
    }

    Bool ParticleSystem::advanceActiveParticle(UInt32 index, Real timeDelta) {
        ParticleStatePtr statePtr = this->particleStates.getParticleStatePtr(index);
        for (UInt32 i = 0; i < this->particleStateOperators.size(); i++) {
            std::shared_ptr<ParticleStateOperator> particleStateOperator = this->particleStateOperators[i];
            particleStateOperator->updateState(statePtr, timeDelta);
            if (*statePtr.age >= *statePtr.lifetime) {
                return false;
            }
        }
        return true;
    }

    void ParticleSystem::copyParticleInArray(UInt32 srcIndex, UInt32 destIndex) {
        this->particleStates.copyParticleState(srcIndex, destIndex);
    }

    ParticleSystem::SystemState ParticleSystem::getSystemState() {
        return this->systemState;
    }

    WeakPointer<ParticleEmitter> ParticleSystem::getEmitter() {
        return this->particleEmitter;
    }

    WeakPointer<ParticleStateInitializer> ParticleSystem::getParticleStateInitializer(UInt32 index) {
        if (index >= this->particleStateInitializers.size()) {
            throw OutOfRangeException("ParticleSystem::getParticleStateInitializer() -> 'index' is out of range.");
        }
        return this->particleStateInitializers[index];
    }

    WeakPointer<ParticleStateOperator> ParticleSystem::getParticleStateOperator(UInt32 index) {
        if (index >= this->particleStateOperators.size()) {
            throw OutOfRangeException("ParticleSystem::getParticleStateOperator() -> 'index' is out of range.");
        }
        return this->particleStateOperators[index];
    }

    UInt32 ParticleSystem::getMaximumActiveParticles() {
        return this->maximumActiveParticles;
    }
}
