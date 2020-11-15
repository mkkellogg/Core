#pragma once

#include <vector>

#include "../util/PersistentWeakPointer.h"
#include "../common/types.h"
#include "../scene/Object3DComponent.h"
#include "ParticleEmitter.h"
#include "ParticleSystemRenderer.h"
#include "ParticleState.h"
#include "initializer/ParticleStateInitializer.h"
#include "operator/ParticleStateOperator.h"

namespace Core {

    class ParticleSystem final: public Object3DComponent {
    public:

        enum class SystemState {
            NotStarted = 0,
            Running = 1,
            Paused = 2,
            Done = 3
        };

        ParticleSystem(WeakPointer<Object3D> owner, UInt32 maximumActiveParticles);
        ~ParticleSystem();

        void update(Real timeDelta);
        void start();
        void pause();
        void stop();

        SystemState getSystemState();

        template <typename T, typename... Args>
        Core::enable_if_t<std::is_base_of<ParticleEmitter, T>::value, Bool>& setEmitter(Args... args) {
            std::shared_ptr<T> emitter = std::make_shared<T>(args...);
            emitter.maxActiveParticles = this->maximumActiveParticles;
            this->particleEmitter = emitter;
            this->emitterInitialized = true;
            return *(emitter.get());
        }
        WeakPointer<ParticleEmitter> getEmitter();

        template <typename T, typename... Args>
        Core::enable_if_t<std::is_base_of<ParticleStateInitializer, T>::value, Bool>& addParticleStateInitializer(Args... args) {
            std::shared_ptr<T> initializer = std::make_shared<T>(args...);
            this->particleStateInitializers.push_back(initializer);
            return *(initializer.get());
        }
        WeakPointer<ParticleStateInitializer> getParticleStateInitializer(UInt32 index);

        template <typename T, typename... Args>
        Core::enable_if_t<std::is_base_of<ParticleStateOperator, T>::value, Bool>& addParticleStateOperator(Args... args) {
            std::shared_ptr<T> particleStateOperator = std::make_shared<T>(args...);
            this->particleStateOperators.push_back(particleStateOperator);
            return *(particleStateOperator.get());
        }
        WeakPointer<ParticleStateOperator> getParticleStateOperator(UInt32 index);

        UInt32 getMaximumActiveParticles();

    private:

        void activateParticles(UInt32 particleCount);
        void activateParticle(UInt32 index);
        void advanceActiveParticles(Real timeDelta);
        Bool advanceActiveParticle(UInt32 index, Real timeDelta);
        void copyParticleInArray(UInt32 srcIndex, UInt32 destIndex);

        Bool emitterInitialized;
        SystemState systemState;
        UInt32 maximumActiveParticles;
        UInt32 activeParticleCount;
        std::shared_ptr<ParticleEmitter> particleEmitter;
        std::vector<std::shared_ptr<ParticleStateInitializer>> particleStateInitializers;
        std::vector<std::shared_ptr<ParticleStateOperator>> particleStateOperators;
        ParticleStateAttributeArray particleStates;
    };
}