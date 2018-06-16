#pragma once

#include <type_traits>

#include "../util/ValueIterator.h"
#include "../common/types.h"
#include "../geometry/Vector3.h"
#include "../math/Matrix4x4.h"
#include "../scene/Object3D.h"
#include "BaseRenderableContainer.h"
#include "ObjectRenderer.h"
#include "Renderable.h"

namespace Core {

    // forward declaration
    class Engine;

    template <typename T, typename Enable = void>
    class RenderableContainer;

    template <typename T>
    class RenderableContainer<T, Core::enable_if_t<std::is_base_of<Renderable<T>, T>::value>> : public BaseRenderableContainer {
        friend class Engine;

    public:
        void addRenderable(WeakPointer<T> renderable) {
            renderables.push_back(renderable);
        }

        const std::vector<WeakPointer<T>> getRenderables() {
            return renderables;
        }

        std::shared_ptr<ObjectRenderer<T>> getRenderer() {
            return renderer;
        }

        ValueIterator<typename std::vector<WeakPointer<T>>::iterator> begin() {
            return this->renderables.begin();
        }

        ValueIterator<typename std::vector<WeakPointer<T>>::iterator> end() {
            return this->renderables.end();
        }

    private:
        RenderableContainer() {
        }

        void setRenderer(std::shared_ptr<ObjectRenderer<T>> renderer) {
            this->setBaseRenderer(std::static_pointer_cast<BaseObjectRenderer>(renderer));
        }

        std::vector<WeakPointer<T>> renderables;
    };
}
