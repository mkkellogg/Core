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
    class ParticleStandardMaterial;

    class ParticleSystemAnimatedSpriteRenderer final: public ParticleSystemRenderer {
        friend class Engine;

    public:
        ~ParticleSystemAnimatedSpriteRenderer() override;
        Bool forwardRender(const ViewDescriptor& viewDescriptor, const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) override;
        Bool forwardRenderObject(const ViewDescriptor& viewDescriptor, WeakPointer<BaseRenderable> renderable, Bool isStatic,
                                         const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) override;
        Bool forwardRenderParticleSystem(const ViewDescriptor& viewDescriptor, WeakPointer<ParticleSystem> particleSystem, Bool isStatic,
                                         const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) override;
        Bool supportsRenderPath(RenderPath renderPath) override;
        UInt32 getRenderQueueID() const override;

        virtual Bool init() override;
        void setRenderState();
        WeakPointer<ParticleStandardMaterial> getMaterial();

    protected:
        ParticleSystemAnimatedSpriteRenderer(WeakPointer<Object3D> owner);

        ParticleStateAttributeArray renderAttributes;
        PersistentWeakPointer<ParticleStandardMaterial> material;
    };
}