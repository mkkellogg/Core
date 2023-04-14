#pragma once

#include "../common/types.h"

namespace Core {
  class Math {
  public:
    static const Real PI;
    static const Real TwoPI;
    static const Real PIOver360;
    static const Real RadsToDegrees;
    static const Real DegreesToRads;

    static Real inverseSquareRoot(Real n);
    static Real quickInverseSquareRoot(Real n);
    static Real squareRoot(Real n);
    static Real quickSquareRoot(Real n);
    static Real round(Real n);
    static Real cos(Real n);
    static Real aCos(Real n);
    static Real sin(Real n);
    static Real aSin(Real n);
    static Real tan(Real n);
    static Real aTan(Real n);
    static Real aTan2(Real n1, Real n2);
    static Real abs(Real n);
    static UInt32 iAbs(Int32 n);

    template <typename T>
    static T min(T a, T b) {
        if (a < b)
            return a;
        else
            return b;
    }

    template <typename T>
    static T max(T a, T b) {
        if (a > b)
            return a;
        else
            return b;
    }

    template <typename T>
    static T clamp(T value, T min, T max) {
        return Math::max(Math::min(value, max), min);
    }

    static Real lerp(Real a, Real b, Real t);
    static Real random();
    static Real sign(Real a);
    static Real pow(Real b, Real e);
  };
}
