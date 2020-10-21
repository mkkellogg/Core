#pragma once

#include "../util/PersistentWeakPointer.h"
#include "../common/types.h"
#include "../render/Object3DRenderer.h"

namespace Core {

    // forward declarations
    class Graphics;
    class ParticleSystem;

    class ParticleSystemRenderer : public Object3DRenderer<ParticleSystem> {
    public:
        ParticleSystemRenderer(WeakPointer<Graphics> graphics, WeakPointer<Object3D> owner);
        virtual ~ParticleSystemRenderer() = 0;

    protected:

        UInt32 maxParticleCount;
    };
}