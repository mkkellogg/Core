#include "SequenceOperator.h"
#include "../../util/Time.h"

namespace Core {

    SequenceOperator::SequenceOperator(WeakPointer<ParticleSequence> particleSequence, Real speed, Bool loop) {
        this->particleSequence = particleSequence;
        this->timeDeltaSinceLastIndexChange = 0.0f;
        this->speed = speed;
        this->loop = loop;
    }

    SequenceOperator::~SequenceOperator() {
    }

    void SequenceOperator::updateState(ParticleStatePtr& state, Real timeDelta) {
        this->timeDeltaSinceLastIndexChange += timeDelta;
        Vector2rs& sequenceElement = *state.sequenceElement;
        if (this->timeDeltaSinceLastIndexChange >= this->speed) {
            Real f = (this->timeDeltaSinceLastIndexChange / this->speed);
            UInt32 iCount = (UInt32)f;
            sequenceElement.x += (Real)iCount;
            if (sequenceElement.x >= this->particleSequence->start + this->particleSequence->length) {
                sequenceElement.x = (Real)this->particleSequence->start;
            }
            this->timeDeltaSinceLastIndexChange = this->timeDeltaSinceLastIndexChange - (Real)(iCount) * this->speed;
        }
    }
}
