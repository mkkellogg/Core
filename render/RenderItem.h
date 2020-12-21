#pragma once

#include <vector>

#include "../common/types.h"
#include "../util/WeakPointer.h"
#include "../util/PersistentWeakPointer.h"

namespace Core {

    // forward declarations
    class BaseObject3DRenderer;
    class BaseRenderable;
    class MeshRenderer;
    class Mesh;
    class ParticleSystemRenderer;
    class ParticleSystem;

    class RenderItem {
    public:
        RenderItem(){
            this->renderer = WeakPointer<BaseObject3DRenderer>::nullPtr();
            this->renderable = WeakPointer<BaseRenderable>::nullPtr();
            this->meshRenderer = WeakPointer<MeshRenderer>::nullPtr();
            this->mesh = WeakPointer<Mesh>::nullPtr();
            this->particleSystemRenderer = WeakPointer<ParticleSystemRenderer>::nullPtr();
            this->particleSystem = WeakPointer<ParticleSystem>::nullPtr();
            this->isStatic = false;
            this->isActive = false;
        }
      
        PersistentWeakPointer<BaseObject3DRenderer> renderer;
        PersistentWeakPointer<BaseRenderable> renderable;
        PersistentWeakPointer<MeshRenderer> meshRenderer;
        PersistentWeakPointer<Mesh> mesh;
        PersistentWeakPointer<ParticleSystemRenderer> particleSystemRenderer;
        PersistentWeakPointer<ParticleSystem> particleSystem;
        Bool isStatic;
        Bool isActive;
        Int32 layer;
    };
}