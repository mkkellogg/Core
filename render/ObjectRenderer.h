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

    protected:
        PersistentWeakPointer<Graphics> graphics;
    };
}
