#include "ParticleStateInitializer.h"
#include "../ParticleSystem.h"

namespace Core {

    ParticleStateInitializer::ParticleStateInitializer() {
    }

    ParticleStateInitializer::~ParticleStateInitializer() {
    }

    void ParticleStateInitializer::initializeState(ParticleStatePtr& state) {
        *state.lifetime = 0.0f;
        *state.age = 0.0f;
        (*state.sequenceElement).set(0, 0);
        (*state.position).set(0.0f, 0.0f, 0.0f);
        (*state.velocity).set(0.0f, 0.0f, 0.0f);
        (*state.normal).set(0.0f, 0.0f, 1.0f);
        *state.rotation = 0.0f;
        *state.rotationalSpeed = 0.0f;
        (*state.size).set(1.0f, 1.0f);
        (*state.color).set(1.0f, 1.0f, 1.0f, 1.0f);
    }
}
