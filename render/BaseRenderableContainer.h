#pragma once

#include "../util/PersistentWeakPointer.h"
#include "../scene/Object3D.h"

namespace Core {

    // forward declarations
    class BaseObjectRenderer;
    class BaseRenderable;

    class BaseRenderableContainer : public Object3D {
    public:
        virtual ~BaseRenderableContainer() = 0;
        WeakPointer<BaseObjectRenderer> getBaseRenderer();

    protected:
        void setBaseRenderer(WeakPointer<BaseObjectRenderer> renderer);
        
        PersistentWeakPointer<BaseObjectRenderer> renderer;
    };
}
