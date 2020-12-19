#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "InterpolatorOperator.h"
#include "../ParticleState.h"
#include "../../geometry/Vector2.h"

namespace Core {

    class SizeInterpolatorOperator: public InterpolatorOperator<Vector2r> {
    public:
        SizeInterpolatorOperator(Bool relativeToInitialValue = false);
        virtual ~SizeInterpolatorOperator();

        virtual Bool updateState(ParticleStatePtr& state, Real timeDelta) override;
    
    };
}