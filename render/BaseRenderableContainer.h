#pragma once

#include "../util/WeakPointer.h"
#include "../scene/Object3D.h"
#include "BaseObjectRenderer.h"

namespace Core {

    class BaseRenderableContainer : public Object3D {
    public:
        virtual ~BaseRenderableContainer() = 0;
        WeakPointer<BaseObjectRenderer> getBaseRenderer();
        void setBaseRenderer(WeakPointer<BaseObjectRenderer> renderer);

    private:
        WeakPointer<BaseObjectRenderer> renderer;
    };
}
