#include "RotationalSpeedInitializer.h"
#include "../../math/Math.h"

namespace Core {

    RotationalSpeedInitializer::RotationalSpeedInitializer(const Generator<Real>& generator) {
        this->generator = std::unique_ptr<Generator<Real>>(generator.clone());
    }

    RotationalSpeedInitializer::~RotationalSpeedInitializer() {
    }

    void RotationalSpeedInitializer::initializeState(ParticleStatePtr& state) {
        Real rotationalSpeed;
        this->generator->generate(rotationalSpeed);
        *state.rotationalSpeed = rotationalSpeed;
    }

}
