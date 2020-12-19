#include <functional>

#include "OpacityInterpolatorOperator.h"
#include "../../util/Time.h"

namespace Core {

    OpacityInterpolatorOperator::OpacityInterpolatorOperator(Bool relativeToInitialValue): InterpolatorOperator(relativeToInitialValue) {
    }

    OpacityInterpolatorOperator::~OpacityInterpolatorOperator() {
    }

    Bool OpacityInterpolatorOperator::updateState(ParticleStatePtr& state, Real timeDelta) {
        if (this->relativeToInitialValue) {
            Real a;
            this->getInterpolatedValue(state, a);
            state.color->a = state.initialColor->a * a;
        } else {
            this->getInterpolatedValue(state, (*state.color).a);
        }
        return true;
    }
}
