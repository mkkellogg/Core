#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "InterpolatorOperator.h"
#include "../ParticleState.h"
#include "../../color/Color.h"

namespace Core {

    class ColorInterpolatorOperator: public InterpolatorOperator<Color> {
    public:
        ColorInterpolatorOperator(Bool ignoreAlpha, Bool relativeToInitialValue = false);
        virtual ~ColorInterpolatorOperator();

        virtual Bool updateState(ParticleStatePtr& state, Real timeDelta) override;
    
    private:

        Bool ignoreAlpha;
    };
}