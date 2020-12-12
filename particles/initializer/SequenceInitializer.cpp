#include "SequenceInitializer.h"
#include "../ParticleSequence.h"
#include "../ParticleSequenceGroup.h"

namespace Core {

    SequenceInitializer::SequenceInitializer() {
    }

    SequenceInitializer::SequenceInitializer(WeakPointer<ParticleSequenceGroup> particleSequences) {
        this->setParticleSequences(particleSequences);
    }

    SequenceInitializer::~SequenceInitializer() {
    }

    void SequenceInitializer::setParticleSequences(WeakPointer<ParticleSequenceGroup> particleSequences) {
        this->particleSequences = particleSequences;
    }

    void SequenceInitializer::initializeState(ParticleStatePtr& state) {
        WeakPointer<ParticleSequence> sequence = this->particleSequences->getSequence(0);
        Vector2rs& sequenceElement = *state.sequenceElement;
        sequenceElement.x = (Real)sequence->start;
        sequenceElement.y = (Real)sequence->id;
    }

}
