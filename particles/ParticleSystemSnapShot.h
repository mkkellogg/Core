#pragma once

#include <vector>

#include "../util/PersistentWeakPointer.h"
#include "../common/types.h"
#include "ParticleState.h"

namespace Core {

    class ParticleSystemSnapShot {
    public:
        ParticleSystemSnapShot();

        void addState(const ParticleState& state);
        const ParticleState& getState(UInt32 index) const;

    private:

        std::vector<ParticleState> particleStates;
    };
}