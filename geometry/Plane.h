#pragma once

#include "Vector3.h"
#include "Vector4.h"

namespace Core {

    class Plane {
    public:
        Plane(Real x, Real y, Real z, Real d);
        Plane(const Vector3r& normal, Real d);
        Plane(const Vector4r& planeEq);

        Vector4r getPlaneEquation() const;

        Point3r projectPoint(const Point3r& point);
        void projectPoint(Point3r& point);

        static Point3r projectPoint(const Vector4r& plane, const Point3r& point);
        static void projectPoint(const Vector4r& plane, Point3r& point);

    private:
        Vector4r planeEq;
    };

}