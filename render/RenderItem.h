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

    class RenderItem {
    public:
        RenderItem(){
            this->renderer = WeakPointer<BaseObject3DRenderer>::nullPtr();
            this->meshRenderer = WeakPointer<MeshRenderer>::nullPtr();
            this->isStatic = false;
            this->isActive = false;
        }
      
        PersistentWeakPointer<BaseObject3DRenderer> renderer;
        PersistentWeakPointer<BaseRenderable> renderable;
        PersistentWeakPointer<MeshRenderer> meshRenderer;
        PersistentWeakPointer<Mesh> mesh;
        Bool isStatic;
        Bool isActive;
    };
}