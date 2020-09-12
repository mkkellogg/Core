#pragma once

#include <vector>

#include "../util/PersistentWeakPointer.h"
#include "../util/ValueIterator.h"
#include "../scene/Object3DComponent.h"

namespace Core {

    // forward declarations
    class BaseObjectRenderer;
    class BaseRenderable;

    class BaseRenderableContainer : public Object3DComponent {
    public:
        virtual ~BaseRenderableContainer() = 0;
        UInt32 getBaseRenderableCount() const;
        WeakPointer<BaseRenderable> getBaseRenderable(UInt32 index);

    protected:
        BaseRenderableContainer(WeakPointer<Object3D> owner);
        void addBaseRenderable(WeakPointer<BaseRenderable> renderable);
        
        PersistentWeakPointer<BaseObjectRenderer> renderer;
        std::vector<PersistentWeakPointer<BaseRenderable>> renderables;
    };
}
