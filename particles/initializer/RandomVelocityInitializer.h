#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "../ParticleState.h"
#include "ParticleStateInitializer.h"

namespace Core {

    class RandomVelocityInitializer: public ParticleStateInitializer {
    public:
        RandomVelocityInitializer(const Vector3r& range, const Vector3r& offset, Real speed);
        RandomVelocityInitializer(Real rangeX, Real rangeY, Real rangeZ, Real offsetX, Real offsetY, Real offsetZ, Real speed);
        virtual ~RandomVelocityInitializer();

        virtual void initializeState(ParticleStatePtr& state) override;

    private:

        Real speed;
        Vector3r range;
        Vector3r offset;
    };
}