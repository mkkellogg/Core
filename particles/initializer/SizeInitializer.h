#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "../ParticleState.h"
#include "ParticleStateInitializer.h"

namespace Core {

    class SizeInitializer: public ParticleStateInitializer {
    public:
        SizeInitializer(Real range, Real offset);
        virtual ~SizeInitializer();

        virtual void initializeState(ParticleStatePtr& state) override;

    private:

        Real sizeRange;
        Real sizeOffset;

    };
}