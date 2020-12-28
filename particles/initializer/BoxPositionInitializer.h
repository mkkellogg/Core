#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "../ParticleState.h"
#include "ParticleStateInitializer.h"
#include "../../geometry/Vector3.h"
#include "../util/RandomGenerator.h"

namespace Core {

    class BoxPositionInitializer: public ParticleStateInitializer {
    public:
        BoxPositionInitializer(const Vector3r& range, const Vector3r& offset);
        BoxPositionInitializer(Real rangeX, Real rangeY, Real rangeZ, Real offsetX, Real offsetY, Real offsetZ);
        virtual ~BoxPositionInitializer();

        virtual void initializeState(ParticleStatePtr& state) override;

    private:

        RandomGenerator<Point3r> randomGenerator;
    };
}