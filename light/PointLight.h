#pragma once

#include "Light.h"

namespace Core {

    // forward declarations
    class Engine;

    class PointLight final : public Light {
        friend class Engine;

    public:

    void setAttenuation(Real attenuation);
    Real setAttenuation() const;

    Real getRadius() const;
    void setRadius(Real radius);
       
    protected:
        PointLight(WeakPointer<Object3D> owner);
        ~PointLight() = 0;

        void calcAttentuationForCurrentRadius();

        Real attenuation;
        Bool attenuationOverride;
        Real radius;
    };
}
