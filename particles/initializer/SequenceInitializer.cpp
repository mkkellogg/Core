#include "SequenceInitializer.h"
#include "../ParticleSequence.h"
#include "../ParticleSequenceGroup.h"

namespace Core {

    SequenceInitializer::SequenceInitializer(WeakPointer<ParticleSequenceGroup> particleSequences): randomDist(0.0f, (Real)particleSequences->getSequenceIDs().size())  {
        std::random_device rd;
        this->mt = std::mt19937(rd());
        this->setParticleSequences(particleSequences);
    }

    SequenceInitializer::~SequenceInitializer() {
    }

    void SequenceInitializer::setParticleSequences(WeakPointer<ParticleSequenceGroup> particleSequences) {
        this->particleSequences = particleSequences;
    }

    void SequenceInitializer::initializeState(ParticleStatePtr& state) {
        const std::vector<UInt32> sequenceIDs = this->particleSequences->getSequenceIDs();
        Real r = randomDist(this->mt);
        if (r == (Real)sequenceIDs.size()) r--;
        UInt32 ir = (UInt32)r;
        UInt32 sequenceID = sequenceIDs[ir];
        WeakPointer<ParticleSequence> sequence = this->particleSequences->getSequence(sequenceID);
        Vector2rs& sequenceElement = *state.sequenceElement;
        sequenceElement.x = (Real)sequence->start;
        sequenceElement.y = (Real)sequence->id;
    }

}
