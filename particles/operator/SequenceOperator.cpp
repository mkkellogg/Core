#include "SequenceOperator.h"
#include "../ParticleSequence.h"
#include "../ParticleSequenceGroup.h"
#include "../../util/Time.h"

namespace Core {

    SequenceOperator::SequenceOperator(WeakPointer<ParticleSequenceGroup> particleSequences, Real speed, Bool loop) {
        this->particleSequences = particleSequences;
        this->timeDeltaSinceLastIndexChange = 0.0f;
        this->speed = speed;
        this->loop = loop;
    }

    SequenceOperator::~SequenceOperator() {
    }

    void SequenceOperator::updateState(ParticleStatePtr& state, Real timeDelta) {
        this->timeDeltaSinceLastIndexChange += timeDelta;
        Vector2rs& sequenceElement = *state.sequenceElement;
        WeakPointer<ParticleSequence> activeSequence = this->particleSequences->getSequence((UInt32)sequenceElement.y);
        if (this->timeDeltaSinceLastIndexChange >= this->speed) {
            Real f = (this->timeDeltaSinceLastIndexChange / this->speed);
            UInt32 iCount = (UInt32)f;
            sequenceElement.x += (Real)iCount;
            if (sequenceElement.x >= activeSequence->start + activeSequence->length) {
                sequenceElement.x = (Real)activeSequence->start;
            }
            this->timeDeltaSinceLastIndexChange = this->timeDeltaSinceLastIndexChange - (Real)(iCount) * this->speed;
        }
    }
}
