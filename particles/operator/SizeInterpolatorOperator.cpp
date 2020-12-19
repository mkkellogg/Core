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
            //std::cerr << std::to_string(state.initialSize->x) << ", " << std::to_string(state.initialSize->y) << std::endl;
            state.size->set(state.initialSize->x * size.x, state.initialSize->y * size.y);
            //std::cerr << std::to_string(state.initialSize->x) << ", " << std::to_string(state.initialSize->y) << std::endl;
            //std::cerr << std::to_string(size.x) << ", " << std::to_string(size.y) << std::endl;
            //std::cerr << "============" << std::endl;
        } else {
            *state.size = size;
        }
        return true;
    }
}
