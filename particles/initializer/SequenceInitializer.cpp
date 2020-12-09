#include "SequenceInitializer.h"
#include "../ParticleSequence.h"

namespace Core {

    SequenceInitializer::SequenceInitializer(WeakPointer<ParticleSequence> particleSequence) {
        this->particleSequence = particleSequence;
    }

    SequenceInitializer::~SequenceInitializer() {
    }

    void SequenceInitializer::initializeState(ParticleStatePtr& state) {
        *state.sequenceElement = this->particleSequence->start;
    }

}
