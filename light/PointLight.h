#pragma once

#include "../util/PersistentWeakPointer.h"
#include "Light.h"
#include "../geometry/Vector3.h"

namespace Core {

    // forward declarations
    class Engine;
    class RenderTargetCube;

    class PointLight final : public Light {
        friend class Engine;

    public:
        ~PointLight();

        void init() override;
        WeakPointer<RenderTarget> getShadowMap() override;
        
        void setAttenuation(Real attenuation);
        Real getAttenuation() const;

        Real getRadius() const;
        void setRadius(Real radius);
      
    protected:
        PointLight(WeakPointer<Object3D> owner);

        void calcAttentuationForCurrentRadius();

        Real attenuation;
        Bool attenuationOverride;
        Real radius;
        PersistentWeakPointer<RenderTargetCube> shadowMap;
    };
}
