#include "Ray.h"
#include "Vector4.h"

namespace Core {

    Ray::Ray(const Vector3Components<Real>& origin, const Vector3Components<Real>& direction) {
        this->origin.copy(origin);
        this->direction.copy(direction);
    }

    Bool Ray::intersectBox(const Box3& box, const Hit& hit) const {

        return false;
    }

    Bool Ray::intersectTriangle(const Vector3Components<Real>& p0, const Vector3Components<Real>& p1,
                                const Vector3Components<Real>& p2, const Hit& hit, const Vector3Components<Real>* normal) const {
        Point3r _p0, _p1, _p2;
        _p0.copy(p0); 
        _p1.copy(p1); 
        _p2.copy(p2);

        Vector3r q1 = _p1 - _p0;
        Vector3r q2 = _p2 - _p0;
        Vector3r _normal;

        if (normal != nullptr) {
            _normal.copy(*normal);
        }
        else {
            Vector3r::cross(q1, q2, _normal);
            _normal.normalize();
        }

        Real d = Vector3r::dot(p0, _normal);
        Vector4r planeEq(_normal.x, _normal.y, _normal.z, d);
        Vector4r rayOrigin(this->origin.x, this->origin.y, this->origin.z, 1.0f);
        Vector4r rayDir(this->direction.x, this->direction.y, this->direction.z, 0.0f);
        Real t = -(Vector4r::dot(planeEq, rayOrigin) / Vector4r::dot(planeEq, rayDir));
        Point3r intersection = this->origin + this->direction * t;

        Vector3r r = intersection - _p0;
        Real rDotQ1 = Vector3r::dot(r, q1);
        Real rDotQ2 = Vector3r::dot(r, q2);

        Real q1Sq = Vector3r::dot(q1, q1);
        Real q2Sq = Vector3r::dot(q2, q2);
        Real q1Dotq2 = Vector3r::dot(q1, q2);

        
        return false;
    }

}