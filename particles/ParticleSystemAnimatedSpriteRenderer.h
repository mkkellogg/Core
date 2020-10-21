#pragma once

#include "../util/PersistentWeakPointer.h"
#include "../common/types.h"
#include "ParticleSystemRenderer.h"
#include "ParticleState.h"

namespace Core {

    // forward declarations
    class Engine;
    class Object3D;
    class Graphics;

    class ParticleSystemAnimatedSpriteRenderer : public ParticleSystemRenderer {
        friend class Engine;

    public:
        virtual ~ParticleSystemAnimatedSpriteRenderer();
        virtual Bool forwardRender(const ViewDescriptor& viewDescriptor, const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) override;
        virtual Bool forwardRenderObject(const ViewDescriptor& viewDescriptor, WeakPointer<BaseRenderable> renderable, Bool isStatic,
                                         const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) override;
        Bool forwardRenderParticleSystem(const ViewDescriptor& viewDescriptor, WeakPointer<ParticleSystem> particleSystem, Bool isStatic,
                                         const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting);
        virtual Bool supportsRenderPath(RenderPath renderPath) override;
        virtual UInt32 getRenderQueueID() const override;

        void init();

    protected:
        ParticleSystemAnimatedSpriteRenderer(WeakPointer<Graphics> graphics, WeakPointer<Object3D> owner);

        ParticleStateAttributeArray particleAttributes;
    };
}