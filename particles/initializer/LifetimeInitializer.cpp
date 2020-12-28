#include "LifetimeInitializer.h"
#include "../../math/Math.h"

namespace Core {

    LifetimeInitializer::LifetimeInitializer(const Generator<Real>& generator) {
        this->generator = std::unique_ptr<Generator<Real>>(generator.clone());
    }

    LifetimeInitializer::~LifetimeInitializer() {
    }

    void LifetimeInitializer::initializeState(ParticleStatePtr& state) {
        Real lifetime;
        this->generator->generate(lifetime);
        *state.lifetime = lifetime;
    }

}
