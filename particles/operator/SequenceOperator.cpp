#include "SequenceOperator.h"
#include "../ParticleSequence.h"
#include "../ParticleSequenceGroup.h"
#include "../../util/Time.h"

namespace Core {

    SequenceOperator::SequenceOperator(WeakPointer<ParticleSequenceGroup> particleSequences, Real speed, Bool loop, Bool reverse) {
        this->particleSequences = particleSequences;
        this->speed = speed;
        this->loop = loop;
        this->reverse = reverse;
    }

    SequenceOperator::~SequenceOperator() {
    }

    Bool SequenceOperator::updateState(ParticleStatePtr& state, Real timeDelta) {
        Vector4rs& sequenceElement = *state.sequenceElement;
        WeakPointer<ParticleSequence> activeSequence = this->particleSequences->getSequence((UInt32)sequenceElement.y);
        Real tdOverS = timeDelta / this->speed;
        if (reverse) {
            sequenceElement.x -= tdOverS;
            if (sequenceElement.x < (Real)activeSequence->start) {
                sequenceElement.x = (Real)activeSequence->start + (Real)activeSequence->length;
                if (!this->loop) return false;
            }
        } else {
            sequenceElement.x += tdOverS;
            if (sequenceElement.x >= (Real)activeSequence->start + (Real)activeSequence->length) {
                sequenceElement.x = (Real)activeSequence->start;
                if (!this->loop) return false;
            }
        }
        return true;
    }
}
