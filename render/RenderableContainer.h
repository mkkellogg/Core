#pragma once

#include <type_traits>

#include "../util/ValueIterator.h"
#include "../common/types.h"
#include "../geometry/Vector3.h"
#include "../math/Matrix4x4.h"
#include "Object3DRenderer.h"
#include "BaseRenderableContainer.h"

namespace Core {

    // forward declaration
    class Engine;
    template <typename T> class Renderable;
    template <typename T> class Object3DRenderer;

    template <typename T, typename Enable = void>
    class RenderableContainer;

    template <typename T>
    class RenderableContainer<T, Core::enable_if_t<std::is_base_of<BaseRenderable, T>::value>> : public BaseRenderableContainer {
        friend class Engine;

    public:
        void addRenderable(WeakPointer<T> renderable) {
            this->addBaseRenderable(renderable);
            this->renderables.push_back(renderable);
        }

        WeakPointer<T> getRenderable(UInt32 index) {
            if (index >= this->renderables.size()) {
                throw OutOfRangeException("RenderableContainer::getRenderable() -> 'index' is out of range.");
            }
            return this->renderables[index];
        }

    protected:
        RenderableContainer(WeakPointer<Object3D> owner): BaseRenderableContainer(owner) {}
        std::vector<WeakPointer<T>> renderables;
    };
}
