#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "ParticleStateOperator.h"
#include "../ParticleState.h"
#include "../ParticleSequence.h"

namespace Core {

    class SequenceOperator: public ParticleStateOperator {
    public:
        SequenceOperator(WeakPointer<ParticleSequence> particleSequence, Real speed, Bool loop);
        virtual ~SequenceOperator();

        virtual void updateState(ParticleStatePtr& state, Real timeDelta) override;
    
    private:

        PersistentWeakPointer<ParticleSequence> particleSequence;
        Real timeDeltaSinceLastIndexChange;
        Real speed;
        Bool loop;

    };
}