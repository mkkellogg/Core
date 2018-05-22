#pragma once

#include "../common/types.h"
#include "Vector3Components.h"
#include "Vector3.h"

namespace Core {

  class Box3 {

    Vector3r min;
    Vector3r max;

  public:

    Box3();
    Box3(const Vector3Components<Real>& min, const Vector3Components<Real>& max);
    Box3(Real minX, Real minY, Real minZ, Real maxX, Real maxY, Real maxZ);

    void setMin(const Vector3Components<Real>& min);
    void setMin(Real x, Real y, Real z);
    void setMax(const Vector3Components<Real>& max);
    void setMax(Real x, Real y, Real z);

    const Box3& operator =(const Box3& other);

    const Vector3r& getMin() const;
    const Vector3r& getMax() const;

    Bool containsBox(const Box3& box) const;
  };

}
