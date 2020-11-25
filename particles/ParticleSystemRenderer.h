#pragma once

#include "../util/PersistentWeakPointer.h"
#include "../common/types.h"
#include "../render/Object3DRenderer.h"

namespace Core {

    // forward declarations
    class Engine;
    class Graphics;
    class ParticleSystem;

    class ParticleSystemRenderer : public Object3DRenderer<ParticleSystem> {
        friend class Engine;
    public:
        virtual Bool forwardRenderParticleSystem(const ViewDescriptor& viewDescriptor, WeakPointer<ParticleSystem> particleSystem, Bool isStatic,
                                         const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) = 0;
        virtual ~ParticleSystemRenderer() = 0;

    protected:
        ParticleSystemRenderer(WeakPointer<Object3D> owner);
        UInt32 maxParticleCount;
    };
}