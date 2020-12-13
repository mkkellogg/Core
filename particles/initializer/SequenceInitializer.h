#pragma once

#include <vector>
#include <random>

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "../ParticleState.h"
#include "ParticleStateInitializer.h"

namespace Core {

    //forward declaration
    class ParticleSequenceGroup;
    class ParticleSequence;

    class SequenceInitializer: public ParticleStateInitializer {
    public:
        SequenceInitializer(WeakPointer<ParticleSequenceGroup> particleSequences);
        virtual ~SequenceInitializer();
;
        void setParticleSequences(WeakPointer<ParticleSequenceGroup> particleSequences);
        virtual void initializeState(ParticleStatePtr& state) override;

    private:

        PersistentWeakPointer<ParticleSequenceGroup> particleSequences;
        std::uniform_real_distribution<Real> randomDist;
        std::mt19937 mt;

    };
}