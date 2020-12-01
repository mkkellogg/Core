#include "BasicParticleStateInitializer.h"
#include "../../math/Math.h"

namespace Core {

    BasicParticleStateInitializer::BasicParticleStateInitializer() {
    }

    BasicParticleStateInitializer::~BasicParticleStateInitializer() {
    }

    void BasicParticleStateInitializer::initializeState(ParticleStatePtr& state) {
        ParticleStateInitializer::initializeState(state);
    }

}
