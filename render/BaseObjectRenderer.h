#pragma once

#include <memory>

#include "../util/WeakPointer.h"
#include "../scene/Object3DComponent.h"
#include "BaseRenderable.h"

namespace Core {

    // forward declarations
    class Object3D;
    class Camera;

    class BaseObjectRenderer : public Object3DComponent {
    public:
        BaseObjectRenderer(WeakPointer<Object3D> owner) : Object3DComponent(owner) {
        }
        virtual void render(WeakPointer<Camera> camera) = 0;
    };
}
