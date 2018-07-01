#pragma once

#include <memory>
#include <vector>

#include "../util/WeakPointer.h"
#include "../scene/Object3DComponent.h"

namespace Core {

    // forward declarations
    class Object3D;
    class Camera;
    class Light;

    class BaseObjectRenderer : public Object3DComponent {
    public:
        BaseObjectRenderer(WeakPointer<Object3D> owner) : Object3DComponent(owner) {
        }
        virtual void render(WeakPointer<Camera> camera, const std::vector<WeakPointer<Light>>& lights) = 0;
    };
}
