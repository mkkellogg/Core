#include <functional>

#include "OpacityInterpolatorOperator.h"
#include "../../util/Time.h"

namespace Core {

    OpacityInterpolatorOperator::OpacityInterpolatorOperator() {
    }

    OpacityInterpolatorOperator::~OpacityInterpolatorOperator() {
    }

    Bool OpacityInterpolatorOperator::updateState(ParticleStatePtr& state, Real timeDelta) {
        this->getInterpolatedValue(state, (*state.color).a);
        return true;
    }
}
