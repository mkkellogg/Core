#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "../ParticleState.h"
#include "ParticleStateInitializer.h"

namespace Core {

    class BoxPositionInitializer: public ParticleStateInitializer {
    public:
        BoxPositionInitializer(const Vector3r& range, const Vector3r& offset);
        BoxPositionInitializer(Real rangeX, Real rangeY, Real rangeZ, Real offsetX, Real offsetY, Real offsetZ);
        virtual ~BoxPositionInitializer();

        virtual void initializeState(ParticleStatePtr& state) override;

    private:

        Vector3r range;
        Vector3r offset;
    };
}