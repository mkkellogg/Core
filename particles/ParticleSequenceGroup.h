#pragma once

#include <vector>

#include "../util/PersistentWeakPointer.h"
#include "../common/types.h"

namespace Core {

    // forward declarations
    class ParticleSequence;

    class ParticleSequenceGroup final {

    public:

        ParticleSequenceGroup();
        ~ParticleSequenceGroup();

        void addSequence(WeakPointer<ParticleSequence> particleSequence);

    private:

         std::vector<PersistentWeakPointer<ParticleSequence>> particleSequences;

    };

}