#pragma once

#include "Generator.h"
#include "../../common/types.h"
#include "../../common/Exception.h"
#include "../../math/Math.h"

namespace Core {

    template <typename T>
    class SphereRandomGenerator final : public Generator<T> {
    public:
        SphereRandomGenerator(Real rangeTheta, Real offsetTheta, Real rangeRadius, Real offsetRadius, Real scaleX, Real scaleY, Real offsetX, Real offsetY):
            SphereRandomGenerator(rangeTheta, offsetTheta, 0.f, 0.0f, rangeRadius, offsetRadius, scaleX, scaleY, 0.0f, offsetX, offsetY, 0.0f) {
        }

        SphereRandomGenerator(Real rangeTheta, Real offsetTheta, Real rangePhi, Real offsetPhi, Real rangeRadius, Real offsetRadius,
                              Real scaleX, Real scaleY, Real scaleZ, Real offsetX, Real offsetY, Real offsetZ) {
            this->rangeTheta = rangeTheta;
            this->offsetTheta = offsetTheta;
            this->rangePhi = rangePhi;
            this->offsetPhi = offsetPhi;
            this->rangeRadius = rangeRadius;
            this->offsetRadius = offsetRadius;

            this->scaleX = scaleX;
            this->scaleY = scaleY;
            this->scaleZ = scaleZ;

            this->offsetX = offsetX;
            this->offsetY = offsetY;
            this->offsetZ = offsetZ;
        }

        ~SphereRandomGenerator() {
        }

        void generate(T& out) override;

        virtual Generator<T>* clone() const override {
            Generator<T>* clone = new(std::nothrow) SphereRandomGenerator<T>(this->rangeTheta, this->offsetTheta, this->rangePhi, this->offsetPhi, this->rangeRadius, this->offsetRadius,
                                                                             this->scaleX, this->scaleY, this->scaleZ, this->offsetX, this->offsetY, this->offsetZ);
            if (clone == nullptr) {
                throw AllocationException("RandomGenerator::clone() -> Unable to allocate clone object.");
            }
            return clone;
        }

    private:

        Real rangeTheta;
        Real offsetTheta;
        Real rangePhi;
        Real offsetPhi;
        Real rangeRadius;
        Real offsetRadius;

        Real scaleX;
        Real scaleY;
        Real scaleZ;
        Real offsetX;
        Real offsetY;
        Real offsetZ;
    };

}