#pragma once

#include <type_traits>

#include "../util/ValueIterator.h"
#include "../common/types.h"
#include "../geometry/Vector3.h"
#include "../math/Matrix4x4.h"
#include "ObjectRenderer.h"
#include "BaseRenderableContainer.h"

namespace Core {

    // forward declaration
    class Engine;
    template <typename T> class Renderable;
    template <typename T> class ObjectRenderer;

    template <typename T, typename Enable = void>
    class RenderableContainer;

    template <typename T>
    class RenderableContainer<T, Core::enable_if_t<std::is_base_of<BaseRenderable, T>::value>> : public BaseRenderableContainer {
        friend class Engine;

    public:
        void addRenderable(WeakPointer<T> renderable) {
            this->addBaseRenderable(renderable);
        }

        WeakPointer<T> getRenderable(UInt32 index) {
            WeakPointer<BaseRenderable> baseRenderable = this->getBaseRenderable(index);
            WeakPointer<T> renderable = WeakPointer<BaseRenderable>::dynamicPointerCast<T>(baseRenderable);
            if (!renderable.isValid() && baseRenderable.isValid()) {
                throw Exception("RenderableContainer::getRenderable could not dynamically cast object.");
            }
            return renderable;
        }

    protected:
        RenderableContainer(WeakPointer<Object3D> owner): BaseRenderableContainer(owner) {}
    };
}
