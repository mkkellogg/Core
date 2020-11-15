#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "../../scene/Object3DComponent.h"
#include "../ParticleState.h"

namespace Core {

    class ParticleStateOperator {
    public:
        ParticleStateOperator();
        virtual ~ParticleStateOperator();

        virtual void updateState(ParticleStatePtr& state, Real timeDelta) = 0;
    };
}