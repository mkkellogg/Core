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

        void setShadowsEnabled(Bool enabled);
        Bool getShadowsEnabled() const;

        void setShadowMapSize(UInt32 size);
        UInt32 getShadowMapSize() const;

    protected:
        ShadowLight(LightType type, WeakPointer<Object3D> owner);

        Bool shadowsEnabled;
        Real shadowBias;
        UInt32 shadowMapSize;
    };
}