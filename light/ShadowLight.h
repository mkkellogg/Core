#pragma once

#include "Light.h"

namespace Core {

    // forward declarations
    class Engine;
    class RenderTarget;

    class ShadowLight : public Light {
        friend class Engine;

    public:
       virtual ~ShadowLight() = 0;

        void setConstantShadowBias(Real bias);
        Real getConstantShadowBias() const;

        void setAngularShadowBias(Real bias);
        Real getAngularShadowBias() const;

        virtual void setShadowsEnabled(Bool enabled);
        Bool getShadowsEnabled() const;

        UInt32 getShadowMapSize() const;

    protected:
        ShadowLight(WeakPointer<Object3D> owner, LightType type, Bool shadowsEnabled, 
                    UInt32 shadowMapSize,  Real constantShadowBias, Real angularShadowBias);

        Bool shadowsEnabled;
        UInt32 shadowMapSize;
        Real constantShadowBias;
        Real angularShadowBias;
    };
}