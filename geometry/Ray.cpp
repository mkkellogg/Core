#include "Ray.h"
#include "Vector4.h"

namespace Core {

    Ray::Ray(const Vector3Components<Real>& origin, const Vector3Components<Real>& direction) {
        this->origin.copy(origin);
        this->direction.copy(direction);
    }

    Bool Ray::intersectBox(const Box3& box, Hit& hit) const {

        const Vector3r& max = box.getMax();
        const Vector3r& min = box.getMin();

        for (UInt32 i = 0; i < 3; i++) {
            Real dir = i == 0 ? this->direction.x : i == 1 ? this->direction.y : this->direction.z;
            Real origin = i == 0 ? this->origin.x : i == 1 ? this->origin.y : this->origin.z;
            Real dirA = i == 0 ? this->origin.y : i == 1 ? this->origin.z : this->origin.x;
            Real dirB = i == 0 ? this->origin.z : i == 1 ? this->origin.x : this->origin.y;
            Vector3r hitNormal = i == 0 ? Vector3r(1.0, 0.0, 0.0) : i == 1 ? Vector3r(0.0, 1.0, 0.0) : Vector3r(0.0, 0.0, 1.0);

            Real extreme = 0.0f, aAtExtreme = 0.0f, bAtExtreme = 0.0f;
            Bool potentialIntersect = false;
            if (dir > 0) {
                extreme = i == 0 ? min.x : i == 1 ? min.y : min.z;
                Real toMin = extreme - origin;
                if (toMin > 0) {
                    aAtExtreme = dirA / dir * toMin;
                    bAtExtreme = dirB / dir * toMin;
                    potentialIntersect = true;
                    hitNormal = hitNormal * -1.0f;
                }
            }
            else if(dir < 0) {
                extreme = i == 0 ? max.x : i == 1 ? max.y : max.z;
                Real toMax = origin - extreme;
                if (toMax > 0) {
                    aAtExtreme = dirA / dir * toMax;
                    bAtExtreme = dirB / dir * toMax;
                    potentialIntersect = true;
                }
            }

            if (potentialIntersect) {
                Real x = i == 0 ? extreme : i == 1 ? bAtExtreme : aAtExtreme;
                Real y = i == 0 ? aAtExtreme : i == 1 ? extreme : bAtExtreme;
                Real z = i == 0 ? bAtExtreme : i == 1 ? aAtExtreme : extreme;
                Real epsilon = 0.0001f;
                if (x >= min.x - epsilon && x <= max.x + epsilon &&
                    y >= min.y - epsilon && y <= max.y + epsilon &&
                    z >= min.z - epsilon && z <= max.z + epsilon) {
                       hit.Origin.set(x, y, z); 
                       hit.Normal = hitNormal;
                       return true ;
                    }
            }
        }

        return false;
    }

    Bool Ray::intersectTriangle(const Vector3Components<Real>& p0, const Vector3Components<Real>& p1,
                                const Vector3Components<Real>& p2, Hit& hit, const Vector3Components<Real>* normal) const {
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

        Real qF = -(q2Sq / q1Dotq2);
        Real w1 = (qF * rDotQ1 + rDotQ2) / (q1Sq * qF + q1Dotq2);
        Real w2 = (rDotQ2 - (w1 * q1Dotq2)) / q2Sq;

        if (w1 < 0 || w2 < 0 || w1 > 1.0 || w2 > 1.0) return false;

        hit.Origin = intersection;
        hit.Normal = _normal;
        
        return true;
    }

}