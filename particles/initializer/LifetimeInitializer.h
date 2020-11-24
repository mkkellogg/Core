#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "../ParticleState.h"
#include "ParticleStateInitializer.h"

namespace Core {

    class LifetimeInitializer: public ParticleStateInitializer {
    public:
        LifetimeInitializer(Real range, Real offset);
        virtual ~LifetimeInitializer();

        virtual void initializeState(ParticleStatePtr& state) override;

    private:

        Real lifetimeRange;
        Real lifetimeOffset;

    };
}