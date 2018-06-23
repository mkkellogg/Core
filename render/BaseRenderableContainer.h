#pragma once

#include "../util/PersistentWeakPointer.h"
#include "../scene/Object3D.h"

namespace Core {

    // forward declarations
    class BaseObjectRenderer;

    class BaseRenderableContainer : public Object3D {
    public:
        virtual ~BaseRenderableContainer() = 0;
        WeakPointer<BaseObjectRenderer> getBaseRenderer();
        void setBaseRenderer(WeakPointer<BaseObjectRenderer> renderer);

    private:
        PersistentWeakPointer<BaseObjectRenderer> renderer;
    };
}
