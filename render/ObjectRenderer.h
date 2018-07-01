#pragma once

#include <memory>

#include "../util/PersistentWeakPointer.h"
#include "BaseObjectRenderer.h"

namespace Core {

    // forward declarations
    class Object3D;
    class Graphics;

    template <typename T>
    class ObjectRenderer : public BaseObjectRenderer {
    public:
        ObjectRenderer(WeakPointer<Graphics> graphics, WeakPointer<Object3D> owner) : BaseObjectRenderer(owner), graphics(graphics) {}
        virtual void renderObject(WeakPointer<Camera> camera, WeakPointer<T> renderable, const std::vector<WeakPointer<Light>>& lights) = 0;

    protected:
        PersistentWeakPointer<Graphics> graphics;
    };
}
