#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "../ParticleState.h"
#include "ParticleStateInitializer.h"

namespace Core {

    class RotationalSpeedInitializer: public ParticleStateInitializer {
    public:
        RotationalSpeedInitializer(Real range, Real offset);
        virtual ~RotationalSpeedInitializer();

        virtual void initializeState(ParticleStatePtr& state) override;

    private:

        Real rotationalSpeedRange;
        Real rotationalSpeedOffset;

    };
}