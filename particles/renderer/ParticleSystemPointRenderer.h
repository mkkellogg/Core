#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "ParticleSystemRenderer.h"
#include "../ParticleState.h"

namespace Core {

    // forward declarations
    class Engine;
    class Object3D;
    class Graphics;
    class Mesh;
    class MeshRenderer;

    class ParticleSystemPointRenderer final: public ParticleSystemRenderer {
        friend class Engine;

    public:
        ~ParticleSystemPointRenderer() override;
        Bool forwardRender(const ViewDescriptor& viewDescriptor, const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) override;
        Bool forwardRenderObject(const ViewDescriptor& viewDescriptor, WeakPointer<BaseRenderable> renderable, Bool isStatic,
                                 Int32 layer, const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) override;
        Bool forwardRenderParticleSystem(const ViewDescriptor& viewDescriptor, WeakPointer<ParticleSystem> particleSystem, Bool isStatic,
                                         Int32 layer, const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) override;
        Bool supportsRenderPath(RenderPath renderPath) override;
        UInt32 getRenderQueueID() const override;

        virtual Bool init() override;

    protected:
        ParticleSystemPointRenderer(WeakPointer<Object3D> owner);
        void updatePointMeshCount(WeakPointer<ParticleSystem> particleSystem);

        std::vector<PersistentWeakPointer<Mesh>> pointMeshes;
        std::vector<PersistentWeakPointer<Object3D>> pointMeshRoots;
        PersistentWeakPointer<Object3D> localRenderRoot;
        PersistentWeakPointer<Object3D> worldRenderRoot;
        ParticleStateAttributeArray renderAttributes;
    };
}