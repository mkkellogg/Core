#include <cmath>
#include "Math.h"
#include "../common/types.h"

#include <math.h>
#include <stdlib.h>

namespace Core {

  const Real Math::PI = 3.14159265358979f;
  const Real Math::TwoPI = Math::PI * 2.0f;
  const Real Math::PIOver360 = Math::PI / 360.0f;
  const Real Math::RadsToDegrees = 360.0f / Math::TwoPI;
  const Real Math::DegreesToRads = Math::TwoPI / 360.0f;

  union IntFloatUnion
  {
    Int32 i;
    Real f;
  };

  Real Math::SquareRoot(Real n)
  {
    return sqrt(n);
  }

  Real Math::QuickSquareRoot(Real n)
  {
    IntFloatUnion ifu;

    Real threeHalfs = 1.5f;
    Real x2 = n * .5f;

    ifu.f = n;
    ifu.i = 0x5f375a86 - (ifu.i >> 1);
    ifu.f = ifu.f * (threeHalfs - (x2 * ifu.f * ifu.f));

    return ifu.f * n;
  }

  Real Math::Round(Real n)
  {
    return (Real)floor(n + 0.5f);
  }

  Real Math::InverseSquareRoot(Real n)
  {
    Real root = SquareRoot(n);
    if (root == 0)return 0;
    return 1 / root;
  }

  Real Math::QuickInverseSquareRoot(Real n)
  {
    Real root = QuickSquareRoot(n);
    if (root == 0)return 0;
    return 1 / root;
  }

  Real Math::Cos(Real n)
  {
    return cos(n);
  }

  Real Math::ACos(Real n)
  {
    return acos(n);
  }

  Real Math::Sin(Real n)
  {
    return sin(n);
  }

  Real Math::Tan(Real n)
  {
    return tan(n);
  }

  Real Math::Abs(Real n)
  {
    return fabs(n);
  }

  Real Math::Min(Real a, Real b)
  {
    if (a < b)return a;
    else return b;
  }

  Int32 Math::Min(Int32 a, Int32 b)
  {
    if(a < b)return a;
    else return b;
  }

  UInt32 Math::Min(UInt32 a, UInt32 b)
  {
    if(a < b)return a;
    else return b;
  }

  Real Math::Max(Real a, Real b)
  {
    if (a > b)return a;
    else return b;
  }

  Int32 Math::Max(Int32 a, Int32 b)
  {
    if(a > b)return a;
    else return b;
  }

  UInt32 Math::Max(UInt32 a, UInt32 b)
  {
    if(a > b)return a;
    else return b;
  }

  Real Math::Lerp(Real a, Real b, Real t)
  {
    return (b - a) * t + a;
  }

  Real Math::Random()
  {
    return (Real)rand() / (Real)RAND_MAX;
  }

}