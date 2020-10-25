#pragma once

#include "../util/PersistentWeakPointer.h"
#include "../common/types.h"
#include "../scene/Object3DComponent.h"
#include "ParticleEmitter.h"
#include "ParticleSystemRenderer.h"
#include "ParticleState.h"

namespace Core {

    class ParticleSystem final: public Object3DComponent {
    public:
        ParticleSystem(WeakPointer<Object3D> owner, UInt32 maximumActiveParticles);
        ~ParticleSystem();

        template <typename T, typename... Args>
        Core::enable_if_t<std::is_base_of<ParticleEmitter, T>::value, Bool>& setEmitter(Args... args) {
            std::shared_ptr<T> emitter = std::make_shared<T>(args...);
            emitter.maxActiveParticles = this->maximumActiveParticles;
            this->particleEmitter = emitter;
            return *(emitter.get());
        }
        WeakPointer<ParticleEmitter> getEmitter();

        UInt32 getMaximumActiveParticles();

    private:

        UInt32 maximumActiveParticles;
        std::shared_ptr<ParticleEmitter> particleEmitter;
        ParticleStateAttributeArray particleStates;
    };
}