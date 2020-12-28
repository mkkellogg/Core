#include "BoxPositionInitializer.h"
#include "../../math/Math.h"

namespace Core {

    BoxPositionInitializer::BoxPositionInitializer(const Vector3r& range, const Vector3r& offset): 
        randomGenerator(Point3r(range.x, range.y, range.z), Point3r(offset.x, offset.y, offset.z), 0.0f,  0.0f, false)  {
    }

    BoxPositionInitializer::BoxPositionInitializer(Real rangeX, Real rangeY, Real rangeZ, Real offsetX, Real offsetY, Real offsetZ):
        BoxPositionInitializer(Vector3r(rangeX, rangeY, rangeZ), Vector3r(offsetX, offsetY, offsetZ)) {
        
    }

    BoxPositionInitializer::~BoxPositionInitializer() {
    }

    void BoxPositionInitializer::initializeState(ParticleStatePtr& state) {
        Point3r position;
        randomGenerator.generate(position);
        *state.position = position;
    }
}
