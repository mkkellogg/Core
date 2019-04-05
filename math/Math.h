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
    static Real tan(Real n);
    static Real aTan(Real n);
    static Real abs(Real n);
    static UInt32 iAbs(Int32 n);
    static Real min(Real a, Real b);
    static Int32 min(Int32 a, Int32 b);
    static UInt32 min(UInt32 a, UInt32 b);
    static Real max(Real a, Real b);
    static Int32 max(Int32 a, Int32 b);
    static UInt32 max(UInt32 a, UInt32 b);
    static Real clamp(Real value, Real min, Real max);
    static Real lerp(Real a, Real b, Real t);
    static Real random();
    static Real sign(Real a);
  };
}
