#include "SphereRandomGenerator.h"
#include "../../geometry/Vector3.h"
#include "../../geometry/Vector2.h"

namespace Core {

    template <>
    void SphereRandomGenerator<Vector2r>::generate(Vector2r& out) {
        
    }

    template <>
    void SphereRandomGenerator<Vector3r>::generate(Vector3r& out) {
        Vector3r up(0.0f, 1.0f, 0.0f);
        Real theta = Math::random() * this->rangeTheta + this->offsetTheta;
        Real phi = Math::random() * this->rangePhi + this->offsetPhi;
        Real thetaX = Math::cos(theta);
        Real thetaY = Math::sin(theta);
        Real phiX = Math::cos(phi);
        Real phiY = Math::sin(phi);

        out.set(thetaX, 0.0f, -thetaY);
        out.scale(phiX);
        up.scale(phiY);
        out.add(up);

        out.normalize();

        Real radius = Math::random() * this->rangeRadius + this->offsetRadius;
        out.scale(radius);

        out.x *= this->scaleX;
        out.y *= this->scaleY;
        out.z *= this->scaleZ;

        out.add(this->offsetX, this->offsetY, this->offsetZ);
    }

    template <>
    void SphereRandomGenerator<Vector3rs>::generate(Vector3rs& out) {
        
    }

    template <>
    void SphereRandomGenerator<Point3r>::generate(Point3r& out) {
        
    }

    template <>
    void SphereRandomGenerator<Point3rs>::generate(Point3rs& out) {
        
    }
}