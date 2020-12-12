#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "ParticleStateOperator.h"
#include "../ParticleState.h"
#include "../ParticleSequence.h"

namespace Core {

    // forward declarations
    class ParticleSequenceGroup;

    class SequenceOperator: public ParticleStateOperator {
    public:
        SequenceOperator(WeakPointer<ParticleSequenceGroup> particleSequences, Real speed, Bool loop);
        virtual ~SequenceOperator();

        virtual void updateState(ParticleStatePtr& state, Real timeDelta) override;
    
    private:

        PersistentWeakPointer<ParticleSequenceGroup> particleSequences;
        Real timeDeltaSinceLastIndexChange;
        Real speed;
        Bool loop;

    };
}