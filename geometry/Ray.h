#pragma once

#include <vector>

#include "../util/WeakPointer.h"
#include "../common/types.h"
#include "../math/Matrix4x4.h"
#include "Vector3.h"
#include "Box3.h"
#include "Hit.h"

namespace Core {

    // forward declarations
    class Mesh;

    class Ray {
    public:
        Ray(const Vector3Components<Real>& origin, const Vector3Components<Real>& direction) {
            this->Origin.set(origin.x, origin.y, origin.z);
            this->Direction.set(direction.x, direction.y, direction.z);
        }
        Bool intersectMesh(WeakPointer<Mesh> mesh, std::vector<Hit>& hits) const;
        Bool intersectBox(const Box3& box, Hit& hit) const;

        Bool intersectTriangle(const Vector3Components<Real>* p0, const Vector3Components<Real>* p1,
                               const Vector3Components<Real>* p2, Hit& hit, const Vector3Components<Real>* normal = nullptr) const ;
        
        Bool intersectTriangle(const Vector3Components<Real>& p0, const Vector3Components<Real>& p1,
                               const Vector3Components<Real>& p2, Hit& hit, const Vector3Components<Real>* normal = nullptr) const;
       
        Bool intersectPlane(Vector4Components<Real>& plane, Hit& hit) const;

        Vector3r Direction;
        Point3r Origin;
    };

}
