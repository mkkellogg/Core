#pragma once

#include <vector>
#include <tuple>

#include "../util/PersistentWeakPointer.h"
#include "ShadowLight.h"
#include "../geometry/Vector3.h"
#include "../math/Matrix4x4.h"
#include "../common/Constants.h"

namespace Core {

    // forward declarations
    class Engine;
    class RenderTarget2D;
    class Camera;

    class DirectionalLight final : public ShadowLight {
        friend class Engine;

    public:
        class OrthoProjection {
        public:
            Real top;   
            Real bottom;
            Real left;
            Real right;
            Real near;
            Real far;
        };

        ~DirectionalLight();

        void init() override;

        void setShadowsEnabled(Bool enabled) override;
        WeakPointer<RenderTarget> getShadowMap(UInt32 cascadeIndex);

        UInt32 getCascadeCount();
        std::vector<OrthoProjection>& buildProjections(WeakPointer<Camera> targetCamera);
        Matrix4x4& getProjectionMatrix(UInt32 cascadeIndex);

        Real getCascadeBoundary(UInt32 boundaryIndex);

    protected:
        DirectionalLight(WeakPointer<Object3D> owner, UInt32 cascadeCount, Bool shadowsEnabled, UInt32 shadowMapSize, Real shadowBias);
        void buildShadowMaps();
        
        std::vector<PersistentWeakPointer<RenderTarget2D>> shadowMaps;
        std::vector<OrthoProjection> projections;
        std::vector<Matrix4x4> projectionMatrices;
        std::vector<Real> cascadeBoundaries;
        UInt32 cascadeCount;
    };
}

