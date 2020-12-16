#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "../ParticleState.h"
#include "ParticleStateInitializer.h"
#include "../../geometry/Vector2.h"

namespace Core {

    class SizeInitializer: public ParticleStateInitializer {
    public:
        SizeInitializer(const Vector2r& range, const Vector2r& offset);
        virtual ~SizeInitializer();

        virtual void initializeState(ParticleStatePtr& state) override;

    private:

        Vector2r sizeRange;
        Vector2r sizeOffset;

    };
}