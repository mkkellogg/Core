#pragma once

#include "../common/types.h"

namespace Core {

    class ParticleSequence final {
        friend class ParticleSystem;

    public:
        UInt32 start;
        UInt32 length;

        ~ParticleSequence();

    private:
        ParticleSequence(UInt32 start, UInt32 length);

    };

}