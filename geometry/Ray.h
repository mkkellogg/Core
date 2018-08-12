#pragma once

#include "../common/types.h"
#include "Vector3.h"
#include "Box3.h"
#include "Hit.h"

namespace Core {

    class Ray {
    public:
        Ray(const Vector3Components<Real>& origin, const Vector3Components<Real>& direction);
        Bool intersectBox(const Box3& box, Hit& hit) const;
        Bool intersectTriangle(const Vector3Components<Real>& p0, const Vector3Components<Real>& p1,
                               const Vector3Components<Real>& p2, Hit& hit, const Vector3Components<Real>* normal = nullptr) const;

    private:
        Vector3r direction;
        Point3r origin;

    };

}
