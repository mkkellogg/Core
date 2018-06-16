#pragma once

#include "../scene/Object3D.h"
#include "BaseObjectRenderer.h"

namespace Core {

    class BaseRenderableContainer : public Object3D {
    public:
        virtual ~BaseRenderableContainer() = 0;
        std::weak_ptr<BaseObjectRenderer> getBaseRenderer();
        void setBaseRenderer(std::shared_ptr<BaseObjectRenderer> renderer);

    private:
        std::weak_ptr<BaseObjectRenderer> renderer;
    };
}
