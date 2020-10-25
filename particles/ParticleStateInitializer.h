#pragma once

#include "../util/PersistentWeakPointer.h"
#include "../common/types.h"
#include "../scene/Object3DComponent.h"
#include "ParticleState.h"

namespace Core {

    class ParticleStateInitializer {
    public:
        ParticleStateInitializer();
        virtual ~ParticleStateInitializer();

        void initializeState(ParticleState& state);

    private:

    };
}