#pragma once

#include <vector>

#include "../util/PersistentWeakPointer.h"
#include "ShadowLight.h"
#include "../geometry/Vector3.h"
#include "../math/Matrix4x4.h"

namespace Core {

    // forward declarations
    class Engine;
    class RenderTarget2D;
    class Camera;

    class DirectionalLight final : public ShadowLight {
        friend class Engine;

    public:
        ~DirectionalLight();

        void init() override;

        void setShadowsEnabled(Bool enabled) override;
        WeakPointer<RenderTarget> getShadowMap(UInt32 cascadeIndex);

        UInt32 getCascadeCount();
        std::vector<Matrix4x4>& buildProjections();

    protected:
        DirectionalLight(WeakPointer<Object3D> owner, UInt32 cascadeCount, WeakPointer<Camera> targetCamera, Bool shadowsEnabled, UInt32 shadowMapSize, Real shadowBias);
        void buildShadowMaps();
        
        WeakPointer<Camera> targetCamera;
        std::vector<PersistentWeakPointer<RenderTarget2D>> shadowMaps;
        std::vector<Matrix4x4> projections;
        UInt32 cascadeCount;
    };
}
