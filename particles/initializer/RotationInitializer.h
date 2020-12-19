#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "../ParticleState.h"
#include "ParticleStateInitializer.h"

namespace Core {

    class RotationInitializer: public ParticleStateInitializer {
    public:
        RotationInitializer(Real range, Real offset);
        virtual ~RotationInitializer();

        virtual void initializeState(ParticleStatePtr& state) override;

    private:

        Real rotationRange;
        Real rotationOffset;

    };
}