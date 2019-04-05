#pragma once

#include <vector>

#include "../util/WeakPointer.h"
#include "../common/types.h"
#include "../math/Matrix4x4.h"
#include "Vector3.h"
#include "Box3.h"
#include "Hit.h"
#include "Plane.h"

namespace Core {

    // forward declarations
    class Mesh;

    class Ray {
    public:
        Ray(const Point3r& origin, const Vector3r& direction) {
            this->Origin.set(origin.x, origin.y, origin.z);
            this->Direction.set(direction.x, direction.y, direction.z);
        }
        Bool intersectMesh(WeakPointer<Mesh> mesh, std::vector<Hit>& hits) const;
        Bool intersectBox(const Box3& box, Hit& hit) const;
        
        Bool intersectTriangle(const Point3r& p0, const Point3r& p1,
                               const Point3r& p2, Hit& hit) const;
       
        Bool intersectPlane(const Vector4r& plane, Hit& hit) const;
        Bool intersectPlane(const Plane& plane, Hit& hit) const;

        Vector3r Direction;
        Point3r Origin;
    };

}
