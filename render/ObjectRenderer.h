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
        virtual Bool forwardRenderObject(const ViewDescriptor& viewDescriptor, WeakPointer<T> renderable,
                                         const std::vector<WeakPointer<Light>>& lights, Bool matchPhysicalPropertiesWithLighting) {
            return false;
        }

    protected:
        PersistentWeakPointer<Graphics> graphics;
    };
}
