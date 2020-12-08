#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "../ParticleState.h"
#include "ParticleStateInitializer.h"

namespace Core {

    //forward declaration
    class ParticleSequence;

    class SequenceInitializer: public ParticleStateInitializer {
    public:
        SequenceInitializer(WeakPointer<ParticleSequence> particleSequence);
        virtual ~SequenceInitializer();

        virtual void initializeState(ParticleStatePtr& state) override;

    private:

        PersistentWeakPointer<ParticleSequence> particleSequence;

    };
}