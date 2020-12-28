#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "../ParticleState.h"
#include "ParticleStateInitializer.h"
#include "../util/Generator.h"

namespace Core {

    class RotationInitializer: public ParticleStateInitializer {
    public:
        RotationInitializer(const Generator<Real>& generator);
        virtual ~RotationInitializer();

        virtual void initializeState(ParticleStatePtr& state) override;

    private:

        std::unique_ptr<Generator<Real>> generator;

    };
}