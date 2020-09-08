#pragma once

#include <vector>

#include "../common/types.h"
#include "../util/WeakPointer.h"
#include "../util/PersistentWeakPointer.h"

namespace Core {

    // forward declarations
    class BaseObjectRenderer;
    class BaseRenderable;

    class RenderItem {
    public:
        RenderItem(){}
        RenderItem(WeakPointer<BaseObjectRenderer> objectRenderer, PersistentWeakPointer<BaseRenderable> renderable, Bool isStatic) {
            this->ObjectRenderer = objectRenderer;
            this->Renderable = renderable;
            this->IsStatic = isStatic;
        }
        PersistentWeakPointer<BaseObjectRenderer> ObjectRenderer;
        PersistentWeakPointer<BaseRenderable> Renderable;
        Bool IsStatic;
    };
}