#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "../ParticleState.h"
#include "ParticleStateInitializer.h"
#include "../../geometry/Vector3.h"
#include "../util/RandomGenerator.h"

namespace Core {

    class RandomVelocityInitializer: public ParticleStateInitializer {
    public:
        RandomVelocityInitializer(const Vector3r& range, const Vector3r& offset, Real speedRange, Real speedOffset);
        RandomVelocityInitializer(Real rangeX, Real rangeY, Real rangeZ, Real offsetX, Real offsetY, Real offsetZ, Real speedRange, Real speedOffset);
        virtual ~RandomVelocityInitializer();

        virtual void initializeState(ParticleStatePtr& state) override;

    private:

        RandomGenerator<Vector3r> directionGenerator;
        RandomGenerator<Real> speedGenerator;
    };
}