#include "ParticleSystemSnapShot.h"
#include "../common/Exception.h"

namespace Core {

    ParticleSystemSnapShot::ParticleSystemSnapShot() {}

    void ParticleSystemSnapShot::addState(const ParticleState& state) {
        this->particleStates.push_back(state);
    }

    const ParticleState& ParticleSystemSnapShot::getState(UInt32 index) const {
        if (index >= this->particleStates.size()) {
            throw OutOfRangeException("ParticleSystemSnapShot::getState -> 'index' is out of range.");
        }
        return this->particleStates[index];
    }
}