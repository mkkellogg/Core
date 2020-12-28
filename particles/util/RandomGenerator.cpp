#include "RandomGenerator.h"
#include "../../geometry/Vector3.h"
#include "../../geometry/Vector2.h"

namespace Core {

    template<>
    RandomGenerator<Real>::RandomGenerator(Real range, Real offset, Bool normalize) {
        this->range = range;
        this->offset = offset;
        this->normalize = normalize;
    }

    template <>
    void RandomGenerator<Real>::generate(Real& out) {
        out = Math::random() * this->range + this->offset;
        if (this->normalize) out = out < 0 ? -1.0f : 1.0f;
    }

    template <>
    void RandomGenerator<Vector2r>::generate(Vector2r& out) {
        Real uniformRange = Math::random() * this->uniformRange;
        out.set(uniformRange + Math::random() * this->range.x + this->offset.x + this->uniformOffset,
                uniformRange + Math::random() * this->range.y + this->offset.y + this->uniformOffset);
        if (this->normalize) out.normalize();
    }

    template <>
    void RandomGenerator<Vector3r>::generate(Vector3r& out) {
        this->generateForThreeElementVector(out);
    }

    template <>
    void RandomGenerator<Vector3rs>::generate(Vector3rs& out) {
        this->generateForThreeElementVector(out);
    }

    template <>
    void RandomGenerator<Point3r>::generate(Point3r& out) {
        this->generateForThreeElementVector(out);
    }

    template <>
    void RandomGenerator<Point3rs>::generate(Point3rs& out) {
        this->generateForThreeElementVector(out);
    }
}