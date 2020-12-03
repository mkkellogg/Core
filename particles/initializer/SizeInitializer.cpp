#include "SizeInitializer.h"
#include "../../math/Math.h"

namespace Core {

    SizeInitializer::SizeInitializer(Real range, Real offset) {
        this->sizeRange = range;
        this->sizeOffset = offset;
    }

    SizeInitializer::~SizeInitializer() {
    }

    void SizeInitializer::initializeState(ParticleStatePtr& state) {
        *state.size = Math::random() * this->sizeRange + this->sizeOffset;
    }

}
