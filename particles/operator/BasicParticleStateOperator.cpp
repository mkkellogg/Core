#include "BasicParticleStateOperator.h"
#include "../../util/Time.h"

namespace Core {

    BasicParticleStateOperator::BasicParticleStateOperator() {
    }

    BasicParticleStateOperator::~BasicParticleStateOperator() {
    }

    void BasicParticleStateOperator::updateState(ParticleStatePtr& state, Real timeDelta) {
        static Vector3r timeScaledVelocity;
        Vector3rs& stateVelocity = *state.velocity;
        timeScaledVelocity.set(stateVelocity.x, stateVelocity.y, stateVelocity.z);
        timeScaledVelocity.multiply(timeDelta);
        state.position->add(timeScaledVelocity.x, timeScaledVelocity.y, timeScaledVelocity.z);

        *state.age = *state.age + timeDelta;

        Real currentRotation = *state.rotation;
        Real currentRotationalSpeed = *state.rotationalSpeed;
        *state.rotation = currentRotation + timeDelta * currentRotationalSpeed;
    }
}
