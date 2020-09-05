#pragma once

#include "../util/PersistentWeakPointer.h"
#include "../util/ValueIterator.h"
#include "../scene/Object3D.h"

namespace Core {

    // forward declarations
    class BaseObjectRenderer;
    class BaseRenderable;

    class BaseRenderableContainer : public Object3D {
    public:
        virtual ~BaseRenderableContainer() = 0;
        WeakPointer<BaseObjectRenderer> getBaseRenderer();
        UInt32 getBaseRenderableCount() const;
        WeakPointer<BaseRenderable> getBaseRenderable(UInt32 index);

    protected:
        void setBaseRenderer(WeakPointer<BaseObjectRenderer> renderer);
        
        PersistentWeakPointer<BaseObjectRenderer> renderer;
        std::vector<PersistentWeakPointer<BaseRenderable>> renderables;
    };
}
