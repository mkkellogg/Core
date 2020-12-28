#include "RotationInitializer.h"
#include "../../math/Math.h"

namespace Core {

    RotationInitializer::RotationInitializer(const Generator<Real>& generator) {
        this->generator = std::unique_ptr<Generator<Real>>(generator.clone());
    }

    RotationInitializer::~RotationInitializer() {
    }

    void RotationInitializer::initializeState(ParticleStatePtr& state) {
        Real rotation;
        this->generator->generate(rotation);
        *state.rotation = rotation;
    }

}
