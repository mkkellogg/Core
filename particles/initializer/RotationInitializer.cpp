#include "RotationInitializer.h"
#include "../../math/Math.h"

namespace Core {

    RotationInitializer::RotationInitializer(Real range, Real offset) {
        this->rotationRange = range;
        this->rotationOffset = offset;
    }

    RotationInitializer::~RotationInitializer() {
    }

    void RotationInitializer::initializeState(ParticleStatePtr& state) {
        *state.rotation = Math::random() * this->rotationRange + this->rotationOffset;
    }

}
