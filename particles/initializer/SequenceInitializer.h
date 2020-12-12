#pragma once

#include <vector>

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "../ParticleState.h"
#include "ParticleStateInitializer.h"

namespace Core {

    //forward declaration
    class ParticleSequenceGroup;

    class SequenceInitializer: public ParticleStateInitializer {
    public:
        SequenceInitializer();
        SequenceInitializer(WeakPointer<ParticleSequence> particleSequence);
        virtual ~SequenceInitializer();

        void addSequence(WeakPointer<ParticleSequence> particleSequence);
        virtual void initializeState(ParticleStatePtr& state) override;

    private:

        std::vector<PersistentWeakPointer<ParticleSequence>> particleSequences;

    };
}