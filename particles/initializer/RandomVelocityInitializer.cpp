#include "RandomVelocityInitializer.h"
#include "../../math/Math.h"

namespace Core {

    RandomVelocityInitializer::RandomVelocityInitializer(const Vector3r& range, const Vector3r& offset, Real speed):
        RandomVelocityInitializer(range.x, range.y, range.z, offset.x, offset.y, offset.z, speed)  {
    }

    RandomVelocityInitializer::RandomVelocityInitializer(Real rangeX, Real rangeY, Real rangeZ, Real offsetX, Real offsetY, Real offsetZ, Real speed) {
        this->range.set(rangeX, rangeY, rangeZ);
        this->offset.set(offsetX, offsetY, offsetZ);
        this->speed = speed;
    }

    RandomVelocityInitializer::~RandomVelocityInitializer() {
    }

    void RandomVelocityInitializer::initializeState(ParticleStatePtr& state) {
        static Vector3r tempVector;
        ParticleStateInitializer::initializeState(state);
        tempVector.x = Math::random() * this->range.x + this->offset.x;
        tempVector.y = Math::random() * this->range.y + this->offset.y;
        tempVector.z = Math::random() * this->range.z + this->offset.z;
        tempVector.normalize();
        tempVector = tempVector * this->speed;
        state.velocity->copy(tempVector);
    }

}
