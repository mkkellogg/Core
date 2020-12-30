#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "ParticleStateOperator.h"
#include "../ParticleState.h"
#include "../../geometry/Vector3.h"
#include "../util/Generator.h"

namespace Core {

    class AccelerationOperator: public ParticleStateOperator {
    public:
        AccelerationOperator(const Generator<Vector3r>& generator);
        virtual ~AccelerationOperator();

        virtual Bool updateState(ParticleStatePtr& state, Real timeDelta) override;
    
    private:

        std::unique_ptr<Generator<Vector3r>> generator;
    };
}