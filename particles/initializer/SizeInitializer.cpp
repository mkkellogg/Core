#include "SizeInitializer.h"
#include "../../math/Math.h"

namespace Core {

    SizeInitializer::SizeInitializer(const Vector2r& range, Real uniformSizeRange, const Vector2r& offset, Real uniformSizeOffset) {
        this->sizeRange = range;
        this->sizeOffset = offset;
        this->uniformSizeRange = uniformSizeRange;
        this->uniformSizeOffset = uniformSizeOffset;
    }

    SizeInitializer::~SizeInitializer() {
    }

    void SizeInitializer::initializeState(ParticleStatePtr& state) {
        Real uniformRange = Math::random() * this->uniformSizeRange;
        (*state.initialSize).set(uniformRange + Math::random() * this->sizeRange.x + this->sizeOffset.x + this->uniformSizeOffset,
                                 uniformRange + Math::random() * this->sizeRange.y + this->sizeOffset.y + this->uniformSizeOffset);
        (*state.size).copy(*state.initialSize);
    }

}
