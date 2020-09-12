#pragma once

#include <vector>

#include "../common/types.h"
#include "../util/WeakPointer.h"
#include "../util/PersistentWeakPointer.h"

namespace Core {

    // forward declarations
    class BaseObjectRenderer;
    class BaseRenderable;
    class MeshRenderer;
    class Mesh;

    class RenderItem {
    public:
        RenderItem(){}
      
        PersistentWeakPointer<BaseObjectRenderer> objectRenderer;
        PersistentWeakPointer<BaseRenderable> renderable;
        PersistentWeakPointer<MeshRenderer> meshRenderer;
        PersistentWeakPointer<Mesh> mesh;
        Bool isStatic;
    };
}