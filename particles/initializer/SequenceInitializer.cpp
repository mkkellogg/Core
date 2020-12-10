#include "SequenceInitializer.h"
#include "../ParticleSequence.h"

namespace Core {

    SequenceInitializer::SequenceInitializer(WeakPointer<ParticleSequence> particleSequence) {
        this->particleSequence = particleSequence;
    }

    SequenceInitializer::~SequenceInitializer() {
    }

    void SequenceInitializer::initializeState(ParticleStatePtr& state) {
        Vector2us& sequenceElement = *state.sequenceElement;
        sequenceElement.x = this->particleSequence->start;
        sequenceElement.y = 0;
    }

}
