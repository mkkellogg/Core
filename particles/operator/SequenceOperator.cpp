#include "SequenceOperator.h"
#include "../ParticleSequence.h"
#include "../ParticleSequenceGroup.h"
#include "../../util/Time.h"

namespace Core {

    SequenceOperator::SequenceOperator(WeakPointer<ParticleSequenceGroup> particleSequences, Real speed, Bool loop) {
        this->particleSequences = particleSequences;
        this->speed = speed;
        this->loop = loop;
    }

    SequenceOperator::~SequenceOperator() {
    }

    Bool SequenceOperator::updateState(ParticleStatePtr& state, Real timeDelta) {
        Vector2rs& sequenceElement = *state.sequenceElement;
        WeakPointer<ParticleSequence> activeSequence = this->particleSequences->getSequence((UInt32)sequenceElement.y);
        Real tdOverS = timeDelta / this->speed;
        sequenceElement.x += tdOverS;
        if (sequenceElement.x >= (Real)activeSequence->start + (Real)activeSequence->length) {
            sequenceElement.x = (Real)activeSequence->start;
            if (!this->loop) return false;
        }
        return true;
    }
}
