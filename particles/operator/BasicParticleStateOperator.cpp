#include "BasicParticleStateOperator.h"
#include "../../util/Time.h"

namespace Core {

    BasicParticleStateOperator::BasicParticleStateOperator() {
    }

    BasicParticleStateOperator::~BasicParticleStateOperator() {
    }

    Bool BasicParticleStateOperator::updateState(ParticleStatePtr& state, Real timeDelta) {
        static Vector3r timeScaledVelocity;
        static Vector3r timeScaledAcceleration;

        Vector3rs& stateAcceleration = *state.acceleration;
        timeScaledAcceleration.set(stateAcceleration.x, stateAcceleration.y, stateAcceleration.z);
        timeScaledAcceleration.scale(timeDelta);
        state.velocity->add(timeScaledAcceleration.x, timeScaledAcceleration.y, timeScaledAcceleration.z);

        Vector3rs& stateVelocity = *state.velocity;
        timeScaledVelocity.set(stateVelocity.x, stateVelocity.y, stateVelocity.z);
        timeScaledVelocity.scale(timeDelta);
       
        state.position->add(timeScaledVelocity.x, timeScaledVelocity.y, timeScaledVelocity.z);

        *state.age = *state.age + timeDelta;

        Real currentRotation = *state.rotation;
        Real currentRotationalSpeed = *state.rotationalSpeed;
        *state.rotation = currentRotation + timeDelta * currentRotationalSpeed;
        return true;
    }
}
