#pragma once

#include "Light.h"

namespace Core {

    // forward declarations
    class Engine;
    class RenderTarget;

    class ShadowLight : public Light {
        friend class Engine;

    public:

        enum class Softness {
            Hard = 0,
            Soft = 1,
            VerySoft = 2
        };

        virtual ~ShadowLight() = 0;

        void setConstantShadowBias(Real bias);
        Real getConstantShadowBias() const;

        void setAngularShadowBias(Real bias);
        Real getAngularShadowBias() const;

        virtual void setShadowsEnabled(Bool enabled);
        Bool getShadowsEnabled() const;

        UInt32 getShadowMapSize() const;

        void setShadowSoftness(Softness softness);
        Softness getShadowSoftness() const;

    protected:
        ShadowLight(WeakPointer<Object3D> owner, LightType type, Bool shadowsEnabled, 
                    UInt32 shadowMapSize,  Real constantShadowBias, Real angularShadowBias);

        Bool shadowsEnabled;
        UInt32 shadowMapSize;
        Real constantShadowBias;
        Real angularShadowBias;
        Softness shadowSoftness;
    };
}