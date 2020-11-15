#pragma once

#include <vector>

#include "../util/PersistentWeakPointer.h"
#include "../common/types.h"

namespace Core {

    //forward declarations
    class ParticleSystem;

    class ParticleSystemManager final {

        friend class Engine;

    public:

        ~ParticleSystemManager();

        void update();
        void addParticleSystem(WeakPointer<ParticleSystem> particleSystem);

    private:

        ParticleSystemManager();

        std::vector<PersistentWeakPointer<ParticleSystem>> particleSystems;
    };
}