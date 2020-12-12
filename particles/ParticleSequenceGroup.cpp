#include "ParticleSequenceGroup.h"

namespace Core {

    ParticleSequenceGroup::ParticleSequenceGroup() {
    }

    ParticleSequenceGroup::~ParticleSequenceGroup() {

    }

    void ParticleSequenceGroup::addSequence(WeakPointer<ParticleSequence> particleSequence) {
        this->particleSequences.push_back(particleSequence);
    }

}