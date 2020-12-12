#include "SequenceInitializer.h"
#include "../ParticleSequence.h"

namespace Core {

    SequenceInitializer() {
    }

    SequenceInitializer::SequenceInitializer(WeakPointer<ParticleSequence> particleSequence) {
        this->addSequence(particleSequence);
    }

    SequenceInitializer::~SequenceInitializer() {
    }

    void SequenceInitializer::addSequence(WeakPointer<ParticleSequence> particleSequence) {
        this->particleSequences.push_back(particleSequence);
    }

    void SequenceInitializer::initializeState(ParticleStatePtr& state) {
        Vector2rs& sequenceElement = *state.sequenceElement;
        sequenceElement.x = (Real)this->particleSequence->start;
        sequenceElement.y = (Real)this->particleSequence->id;
    }

}
