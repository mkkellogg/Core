#include "ParticleSequence.h"

namespace Core {

    ParticleSequence::ParticleSequence(UInt32 start, UInt32 length): ParticleSequence(0, start, length) {
    }

    ParticleSequence::ParticleSequence(UInt32 id, UInt32 start, UInt32 length) {
        this->start = start;
        this->length = length;
        this->id = id;
    }

    ParticleSequence::~ParticleSequence() {

    }

}