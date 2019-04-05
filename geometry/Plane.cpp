#include "Plane.h"

namespace Core {

    Plane::Plane(Real x, Real y, Real z, Real d) {
        Real mag = x * x + y * y + z * z;
        x /= mag;
        y /= mag;
        z /= mag;
        d /= mag;
        this->planeEq.set(x, y, z, d);
    }

    Plane::Plane(const Vector3r& normal, Real d): Plane(normal.x, normal.y, normal.z, d) {

    }

    Plane::Plane(const Vector4r& eq): Plane(eq.x, eq.y, eq.z, eq.w) {

    }

    Vector4r Plane::getPlaneEquation() const {
        return this->planeEq;
    }

    Point3r Plane::projectPoint(const Point3r& point) {
        return projectPoint(this->planeEq, point);
    }

    void Plane::projectPoint(Point3r& point) {
        projectPoint(this->planeEq, point);
    }

    Point3r Plane::projectPoint(const Vector4r& plane, const Point3r& point) {
        Point3r copy = point;
        projectPoint(plane, copy);
        return copy;
    }

    void Plane::projectPoint(const Vector4r& plane, Point3r& point) {
        Vector4r point4;
        point4.set(point.x, point.y, point.z, 1.0f);

        Real offsetScale = plane.dot(point4);
        point.x -= offsetScale * plane.x;
        point.y -= offsetScale * plane.y;
        point.z -= offsetScale * plane.z;
    }

}