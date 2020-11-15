#include "ParticleStateInitializer.h"

namespace Core {

    ParticleStateInitializer::ParticleStateInitializer() {
    }

    ParticleStateInitializer::~ParticleStateInitializer() {
    }

    void ParticleStateInitializer::initializeState(ParticleStatePtr& state) {
        *state.lifetime = 1.0f;
        *state.age = 0.0f;
        *state.sequenceNumber = 0;
        (*state.position).set(0.0f, 0.0f, 0.0f);
        (*state.velocity).set(0.0f, 0.0f, 0.0f);
        (*state.normal).set(0.0f, 0.0f, 1.0f);
        *state.rotation = 0.0f;
        *state.rotationalSpeed = 0.0f;
        *state.radius = 1.0f;
        (*state.color).set(1.0f, 1.0f, 1.0f, 1.0f);
    }
}
