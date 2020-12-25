#include <functional>

#include "SizeInterpolatorOperator.h"
#include "../../util/Time.h"

namespace Core {

    SizeInterpolatorOperator::SizeInterpolatorOperator(Bool relativeToInitialValue): InterpolatorOperator(relativeToInitialValue) {
    }

    SizeInterpolatorOperator::~SizeInterpolatorOperator() {
    }

    Bool SizeInterpolatorOperator::updateState(ParticleStatePtr& state, Real timeDelta) {
        Vector2r size;
        this->getInterpolatedValue(state, size);
        if (this->relativeToInitialValue) {
            state.size->set(state.initialSize->x * size.x, state.initialSize->y * size.y);
        } else {
            *state.size = size;
        }
        return true;
    }
}
