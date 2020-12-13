#pragma once

#include <unordered_map>
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

        WeakPointer<ParticleSequence> addParticleSequence(UInt32 id, UInt32 start, UInt32 length);
        WeakPointer<ParticleSequence> getSequence(UInt32 id);
        const std::vector<UInt32>& getSequenceIDs();

    private:
        Bool hasID(UInt32 id);

        std::unordered_map<UInt32, std::shared_ptr<ParticleSequence>> particleSequences;
        std::vector<UInt32> ids;

    };

}