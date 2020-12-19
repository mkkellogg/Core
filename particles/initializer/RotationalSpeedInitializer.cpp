#include "RotationalSpeedInitializer.h"
#include "../../math/Math.h"

namespace Core {

    RotationalSpeedInitializer::RotationalSpeedInitializer(Real range, Real offset) {
        this->rotationalSpeedRange = range;
        this->rotationalSpeedOffset = offset;
    }

    RotationalSpeedInitializer::~RotationalSpeedInitializer() {
    }

    void RotationalSpeedInitializer::initializeState(ParticleStatePtr& state) {
        *state.rotationalSpeed = Math::random() * this->rotationalSpeedRange + this->rotationalSpeedOffset;
    }

}
