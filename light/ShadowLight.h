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

        void setShadowBias(Real bias);
        Real getShadowBias() const;

        virtual void setShadowsEnabled(Bool enabled);
        Bool getShadowsEnabled() const;

        UInt32 getShadowMapSize() const;

    protected:
        ShadowLight(WeakPointer<Object3D> owner, LightType type, Bool shadowsEnabled, UInt32 shadowMapSize, Real shadowBias);

        Bool shadowsEnabled;
        Real shadowBias;
        UInt32 shadowMapSize;
    };
}