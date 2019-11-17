#pragma once

#include "../util/PersistentWeakPointer.h"
#include "ShadowLight.h"
#include "../geometry/Vector3.h"

namespace Core {

    // forward declarations
    class Engine;
    class RenderTargetCube;

    class PointLight final : public ShadowLight {
        friend class Engine;

    public:
        static const Real NearPlane;
        static const Real FarPlane;

        virtual ~PointLight();

        void init() override;

        void setShadowsEnabled(Bool enabled) override;
        WeakPointer<RenderTarget> getShadowMap();
        
        void setAttenuation(Real attenuation);
        Real getAttenuation() const;

        Real getRadius() const;
        void setRadius(Real radius);
      
    protected:
        PointLight(WeakPointer<Object3D> owner, Bool shadowsEnabled, 
                   UInt32 shadowMapSize, Real constantShadowBias, Real angularShadowBias);

        void calcAttentuationForCurrentRadius();
        void buildShadowMap();

        Real attenuation;
        Bool attenuationOverride;
        Real radius;

        PersistentWeakPointer<RenderTargetCube> shadowMap;
    };
}
