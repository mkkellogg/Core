#pragma once

#include <vector>

#include "../util/PersistentWeakPointer.h"
#include "ShadowLight.h"
#include "../geometry/Vector3.h"

namespace Core {

    // forward declarations
    class Engine;
    class RenderTarget2D;

    class DirectionalLight final : public ShadowLight {
        friend class Engine;

    public:
        ~DirectionalLight();

        void init() override;
        WeakPointer<RenderTarget> getShadowMap(UInt32 cascadeIndex);

        UInt32 getCascadeCount();

    protected:
        DirectionalLight(WeakPointer<Object3D> owner, UInt32 cascadeCount, Bool shadowsEnabled, UInt32 shadowMapSize, Real shadowBias);

        UInt32 cascadeCount;
        std::vector<PersistentWeakPointer<RenderTarget2D>> shadowMaps;
    };
}
