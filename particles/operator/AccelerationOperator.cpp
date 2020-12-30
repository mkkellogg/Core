#include "AccelerationOperator.h"
#include "../../util/Time.h"

namespace Core {

    AccelerationOperator::AccelerationOperator(const Generator<Vector3r>& generator) {
        this->generator = std::unique_ptr<Generator<Vector3r>>(generator.clone());
    }

    AccelerationOperator::~AccelerationOperator() {
    }

    Bool AccelerationOperator::updateState(ParticleStatePtr& state, Real timeDelta) {
        Vector3r acceleration;
        this->generator->generate(acceleration);
        state.acceleration->copy(acceleration);
        return true;
    }
}
