#include "ParticleSequenceGroup.h"
#include "ParticleSequence.h"

namespace Core {

    ParticleSequenceGroup::ParticleSequenceGroup() {
    }

    ParticleSequenceGroup::~ParticleSequenceGroup() {

    }

    WeakPointer<ParticleSequence> ParticleSequenceGroup::addParticleSequence(UInt32 id, UInt32 start, UInt32 length) {
        if (this->hasID(id)) {
            throw Exception("ParticleSequenceGroup::addParticleSequence -> Tried to add sequence with duplicate ID.");
        }

        ParticleSequence* sequencePtr = new(std::nothrow) ParticleSequence(id, start, length);
        if (sequencePtr == nullptr) {
            throw AllocationException("ParticleSystem::createParticleSequence -> Could not allocate new particle sequence.");
        }
        std::shared_ptr<ParticleSequence> sequence = std::shared_ptr<ParticleSequence>(sequencePtr);
        this->particleSequences[id] = sequence;
        this->ids.push_back(id);
        return sequence;
    }

    WeakPointer<ParticleSequence> ParticleSequenceGroup::getSequence(UInt32 id) {
        if (!this->hasID(id)) {
            throw InvalidArgumentException("ParticleSequenceGroup::getSequence -> Invalid ID.");
        }
        return this->particleSequences[id];
    }

    const std::vector<UInt32>& ParticleSequenceGroup::getSequenceIDs() {
        return this->ids;
    }

    Bool ParticleSequenceGroup::hasID(UInt32 id) {
        return !(this->particleSequences.find(id) == this->particleSequences.end());
    }

}