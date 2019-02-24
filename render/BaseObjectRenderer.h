#pragma once

#include <memory>
#include <vector>

#include "../util/WeakPointer.h"
#include "../scene/Object3DComponent.h"
#include "RenderPath.h"
#include "ViewDescriptor.h"

namespace Core {

    // forward declarations
    class Object3D;
    class Camera;
    class Light;

    class BaseObjectRenderer : public Object3DComponent {
    public:
        BaseObjectRenderer(WeakPointer<Object3D> owner) : Object3DComponent(owner), castShadows(true) {}
        virtual Bool forwardRender(const ViewDescriptor& viewDescriptor, const std::vector<WeakPointer<Light>>& lights);
        virtual Bool supportsRenderPath(RenderPath renderPath);
        Bool castsShadows();
        void setCastShadows(Bool castShadows);

    private:

        Bool castShadows;;
    };
}
