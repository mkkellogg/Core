#pragma once

#include <memory>

#include "../util/PersistentWeakPointer.h"
#include "BaseObject3DRenderer.h"

namespace Core {

    // forward declarations
    class Object3D;
    class Graphics;

    template <typename T>
    class Object3DRenderer : public BaseObject3DRenderer {
    public:
        Object3DRenderer(WeakPointer<Object3D> owner) : BaseObject3DRenderer(owner) {}
    };
}
