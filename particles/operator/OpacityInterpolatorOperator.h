#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "InterpolatorOperator.h"
#include "../ParticleState.h"

namespace Core {

    class OpacityInterpolatorOperator: public InterpolatorOperator<Real> {
    public:
        OpacityInterpolatorOperator();
        virtual ~OpacityInterpolatorOperator();

        virtual Bool updateState(ParticleStatePtr& state, Real timeDelta) override;
    
    };
}