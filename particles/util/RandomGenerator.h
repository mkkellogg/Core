#pragma once

#include "Generator.h"
#include "../../common/types.h"
#include "../../common/Exception.h"
#include "../../math/Math.h"

namespace Core {

    template <typename T>
    class RandomGenerator final : public Generator<T> {
    public:
        RandomGenerator(Real range, Real offset, Bool normalize);
        RandomGenerator(const T& range, const T& offset, const Real uniformRange, const Real uniformOffset, Bool normalize) {
            this->range = range;
            this->offset = offset;
            this->uniformRange = uniformRange;
            this->uniformOffset = uniformOffset;
            this->normalize = normalize;
        }

        ~RandomGenerator() {
        }

        void generate(T& out) override;

        virtual Generator<T>* clone() const override {
            Generator<T>* clone = new(std::nothrow) RandomGenerator<T>(this->range, this->offset, this->uniformRange, this->uniformOffset, this->normalize);
            if (clone == nullptr) {
                throw AllocationException("RandomGenerator::clone() -> Unable to allocate clone object.");
            }
            return clone;
        }

    private:

        template <typename V>
        void generateForThreeElementVector(V& out) {
            Real uniformRange = Math::random() * this->uniformRange;
            out.set(uniformRange + Math::random() * this->range.x + this->offset.x + this->uniformOffset,
                    uniformRange + Math::random() * this->range.y + this->offset.y + this->uniformOffset,
                    uniformRange + Math::random() * this->range.z + this->offset.z + this->uniformOffset);
            if (this->normalize) out.normalize();
        }

        T range;
        T offset;
        Real uniformRange;
        Real uniformOffset;
        Bool normalize;
    };

}