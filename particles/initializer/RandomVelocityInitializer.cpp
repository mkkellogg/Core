#include "RandomVelocityInitializer.h"
#include "../../math/Math.h"

namespace Core {

    RandomVelocityInitializer::RandomVelocityInitializer(const Vector3r& range, const Vector3r& offset, Real speedRange, Real speedOffset):
        directionGenerator(range, offset, 0.0f, 0.0f, true), speedGenerator(speedRange, speedOffset, false)  {
    }

    RandomVelocityInitializer::RandomVelocityInitializer(Real rangeX, Real rangeY, Real rangeZ,
                                                         Real offsetX, Real offsetY, Real offsetZ,
                                                         Real speedRange, Real speedOffset):
        RandomVelocityInitializer(Vector3r(rangeX, rangeY, rangeZ), Vector3r(offsetX, offsetY, offsetZ), speedRange, speedOffset) {
    }

    RandomVelocityInitializer::~RandomVelocityInitializer() {
    }

    void RandomVelocityInitializer::initializeState(ParticleStatePtr& state) {
        Vector3r tempVector;
        Real speed;
        directionGenerator.generate(tempVector);
        speedGenerator.generate(speed);
        state.velocity->copy(tempVector);
        state.velocity->scale(speed);
    }

}
