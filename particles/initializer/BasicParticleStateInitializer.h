#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "../ParticleState.h"
#include "ParticleStateInitializer.h"

namespace Core {

    class BasicParticleStateInitializer: public ParticleStateInitializer {
    public:
        BasicParticleStateInitializer();
        virtual ~BasicParticleStateInitializer();

        virtual void initializeState(ParticleStatePtr& state) override;

    private:

    };
}