#include "SequenceInitializer.h"
#include "../ParticleSequence.h"
#include "../ParticleSequenceGroup.h"

namespace Core {

    SequenceInitializer::SequenceInitializer(WeakPointer<ParticleSequenceGroup> particleSequences, Bool reverse):
        randomDist(0.0f, (Real)particleSequences->getSequenceIDs().size())  {
        std::random_device rd;
        this->mt = std::mt19937(rd());
        this->setParticleSequences(particleSequences);
        this->reverse = reverse;
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
        Vector3rs& sequenceElement = *state.sequenceElement;
        if (reverse) sequenceElement.x = (Real)(sequence->length - 1);
        else sequenceElement.x = (Real)sequence->start;
        sequenceElement.y = (Real)sequence->id;
        sequenceElement.z = (Real)sequence->length;
        *state.progressType = (Real)ParticleStateProgressType::Sequence;
    }

}
