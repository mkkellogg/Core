#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "ParticleStateOperator.h"
#include "../ParticleState.h"

namespace Core {

    class BasicParticleStateOperator: public ParticleStateOperator {
    public:
        BasicParticleStateOperator();
        virtual ~BasicParticleStateOperator();

        virtual void updateState(ParticleStatePtr& state, Real timeDelta) override;
    
    private:


    };
}