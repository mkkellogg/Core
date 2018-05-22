//
// Created by Mark Kellogg on 12/16/17.
//

#ifndef MPM_MATH_H
#define MPM_MATH_H

#include "../common/types.h"

namespace Core {
  class Math {
  public:
    static const Real PI;
    static const Real TwoPI;
    static const Real PIOver360;
    static const Real RadsToDegrees;
    static const Real DegreesToRads;

    static Real InverseSquareRoot(Real n);
    static Real QuickInverseSquareRoot(Real n);
    static Real SquareRoot(Real n);
    static Real QuickSquareRoot(Real n);
    static Real Round(Real n);
    static Real Cos(Real n);
    static Real ACos(Real n);
    static Real Sin(Real n);
    static Real Tan(Real n);
    static Real Abs(Real n);
    static Real Min(Real a, Real b);
    static Int32 Min(Int32 a, Int32 b);
    static UInt32 Min(UInt32 a, UInt32 b);
    static Real Max(Real a, Real b);
    static Int32 Max(Int32 a, Int32 b);
    static UInt32 Max(UInt32 a, UInt32 b);
    static Real Lerp(Real a, Real b, Real t);
    static Real Random();
  };
}


#endif //MPM_MATH_H
