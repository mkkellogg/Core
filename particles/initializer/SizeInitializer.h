#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "../ParticleState.h"
#include "ParticleStateInitializer.h"
#include "../../geometry/Vector2.h"
#include "../util/Generator.h"

namespace Core {

    class SizeInitializer: public ParticleStateInitializer {
    public:
        SizeInitializer(const Generator<Vector2r>& generator);
        virtual ~SizeInitializer();

        virtual void initializeState(ParticleStatePtr& state) override;

    private:

        std::unique_ptr<Generator<Vector2r>> generator;
    };
}