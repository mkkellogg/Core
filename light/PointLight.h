#pragma once

#include "Light.h"
#include "../geometry/Vector3.h"

namespace Core {

    // forward declarations
    class Engine;

    class PointLight final : public Light {
        friend class Engine;

    public:
        ~PointLight();
        
        void setAttenuation(Real attenuation);
        Real setAttenuation() const;

        Real getRadius() const;
        void setRadius(Real radius);

        void setPosition(const Point3<Real>& postion);
        void setPosition(Real x, Real y, Real z);
        Point3r getPosition();
       
    protected:
        PointLight(WeakPointer<Object3D> owner);

        void calcAttentuationForCurrentRadius();

        Real attenuation;
        Bool attenuationOverride;
        Real radius;
        Point3r position;
    };
}
