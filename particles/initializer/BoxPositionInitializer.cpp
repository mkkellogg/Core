#include "BoxPositionInitializer.h"
#include "../../math/Math.h"

namespace Core {

    BoxPositionInitializer::BoxPositionInitializer(const Vector3r& range, const Vector3r& offset):
        BoxPositionInitializer(range.x, range.y, range.z, offset.x, offset.y, offset.z)  {
    }

    BoxPositionInitializer::BoxPositionInitializer(Real rangeX, Real rangeY, Real rangeZ, Real offsetX, Real offsetY, Real offsetZ) {
        this->range.set(rangeX, rangeY, rangeZ);
        this->offset.set(offsetX, offsetY, offsetZ);
    }

    BoxPositionInitializer::~BoxPositionInitializer() {
    }

    void BoxPositionInitializer::initializeState(ParticleStatePtr& state) {
        ParticleStateInitializer::initializeState(state);
        state.position->x = Math::random() * this->range.x + this->offset.x;
        state.position->y = Math::random() * this->range.y + this->offset.y;
        state.position->z = Math::random() * this->range.z + this->offset.z;
    }
}
