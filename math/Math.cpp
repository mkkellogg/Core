#include "Math.h"
#include <cmath>
#include "../common/types.h"

#include <math.h>
#include <stdlib.h>

namespace Core {

    class MathProxy {
    public:
        static Core::Real _sin(Core::Real a) { return sin(a); }
        static Core::Real _cos(Core::Real a) { return cos(a); }
        static Core::Real _tan(Core::Real a) { return tan(a); }
        static Core::Real _atan(Core::Real a) { return atan(a); }
    };

    const Real Math::PI = 3.14159265358979f;
    const Real Math::TwoPI = Math::PI * 2.0f;
    const Real Math::PIOver360 = Math::PI / 360.0f;
    const Real Math::RadsToDegrees = 360.0f / Math::TwoPI;
    const Real Math::DegreesToRads = Math::TwoPI / 360.0f;

    union IntFloatUnion {
        Int32 i;
        Real f;
    };

    Real Math::squareRoot(Real n) {
        return sqrt(n);
    }

    Real Math::quickSquareRoot(Real n) {
        IntFloatUnion ifu;

        Real threeHalfs = 1.5f;
        Real x2 = n * .5f;

        ifu.f = n;
        ifu.i = 0x5f375a86 - (ifu.i >> 1);
        ifu.f = ifu.f * (threeHalfs - (x2 * ifu.f * ifu.f));

        return ifu.f * n;
    }

    Real Math::round(Real n) {
        return (Real)floor(n + 0.5f);
    }

    Real Math::inverseSquareRoot(Real n) {
        Real root = squareRoot(n);
        if (root == 0) return 0;
        return 1 / root;
    }

    Real Math::quickInverseSquareRoot(Real n) {
        Real root = quickSquareRoot(n);
        if (root == 0) return 0;
        return 1 / root;
    }

    Real Math::cos(Real n) {
        return MathProxy::_cos(n);
    }

    Real Math::aCos(Real n) {
        return acos(n);
    }

    Real Math::sin(Real n) {
        return MathProxy::_sin(n);
    }

    Real Math::tan(Real n) {
        return MathProxy::_tan(n);
    }

    Real Math::aTan(Real n) {
        return MathProxy::_atan(n);
    }

    Real Math::abs(Real n) {
        return fabs(n);
    }

    UInt32 Math::iAbs(Int32 n) {
        return n >= 0 ? n : -n;
    }

    Real Math::min(Real a, Real b) {
        if (a < b)
            return a;
        else
            return b;
    }

    Int32 Math::min(Int32 a, Int32 b) {
        if (a < b)
            return a;
        else
            return b;
    }

    UInt32 Math::min(UInt32 a, UInt32 b) {
        if (a < b)
            return a;
        else
            return b;
    }

    Real Math::max(Real a, Real b) {
        if (a > b)
            return a;
        else
            return b;
    }

    Int32 Math::max(Int32 a, Int32 b) {
        if (a > b)
            return a;
        else
            return b;
    }

    UInt32 Math::max(UInt32 a, UInt32 b) {
        if (a > b)
            return a;
        else
            return b;
    }

    Real Math::clamp(Real value, Real min, Real max) {
        return Math::max(Math::min(value, max), min);
    }

    Real Math::lerp(Real a, Real b, Real t) {
        return (b - a) * t + a;
    }

    Real Math::random() {
        return (Real)rand() / (Real)RAND_MAX;
    }

    Real Math::sign(Real a) {
        return a < 0.0f ? -1.0f : 1.0f;
    }
}
