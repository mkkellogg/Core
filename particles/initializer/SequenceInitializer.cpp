#include "SequenceInitializer.h"
#include "../ParticleSequence.h"

namespace Core {

    SequenceInitializer::SequenceInitializer(WeakPointer<ParticleSequence> particleSequence) {
        this->particleSequence = particleSequence;
    }

    SequenceInitializer::~SequenceInitializer() {
    }

    void SequenceInitializer::initializeState(ParticleStatePtr& state) {
        Vector2rs& sequenceElement = *state.sequenceElement;
        sequenceElement.x = (Real)this->particleSequence->start;
        sequenceElement.y = (Real)this->particleSequence->id;
    }

}
