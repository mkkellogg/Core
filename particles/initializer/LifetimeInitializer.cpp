#include "LifetimeInitializer.h"
#include "../../math/Math.h"

namespace Core {

    LifetimeInitializer::LifetimeInitializer(Real range, Real offset) {
        this->lifetimeRange = range;
        this->lifetimeOffset = offset;
    }

    LifetimeInitializer::~LifetimeInitializer() {
    }

    void LifetimeInitializer::initializeState(ParticleStatePtr& state) {
        *state.lifetime = Math::random() * this->lifetimeRange + this->lifetimeOffset;
    }

}
