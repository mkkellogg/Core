#include "SizeInitializer.h"
#include "../../math/Math.h"

namespace Core {

    SizeInitializer::SizeInitializer(const Vector2r& range, const Vector2r& offset) {
        this->sizeRange = range;
        this->sizeOffset = offset;
    }

    SizeInitializer::~SizeInitializer() {
    }

    void SizeInitializer::initializeState(ParticleStatePtr& state) {
        (*state.size).set(Math::random() * this->sizeRange.x + this->sizeOffset.x, Math::random() * this->sizeRange.y + this->sizeOffset.y);
    }

}
