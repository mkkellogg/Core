#pragma once

#include "ParticleStateInitializer.h"
#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "../ParticleState.h"
#include "../util/Generator.h"

namespace Core {

    class RotationalSpeedInitializer: public ParticleStateInitializer {
    public:
        RotationalSpeedInitializer(const Generator<Real>& generator);
        virtual ~RotationalSpeedInitializer();

        virtual void initializeState(ParticleStatePtr& state) override;

    private:

        std::unique_ptr<Generator<Real>> generator;

    };
}