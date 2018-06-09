#pragma once

#include <type_traits>

#include "../common/types.h"
#include "../math/Matrix4x4.h"
#include "../scene/Object3D.h"
#include "../geometry/Vector3.h"
#include "ObjectRenderer.h"
#include "BaseRenderableContainer.h"
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
   
    void addRenderable(std::shared_ptr<T> renderable) {
      renderables.push_back(renderable);
    }

    const std::vector<std::shared_ptr<T>> getRenderables() {
      return renderables;
    }

    std::shared_ptr<ObjectRenderer<T>> getRenderer() {
      return renderer;
    }

    typename std::vector<std::shared_ptr<T>>::iterator begin() {
      return this->renderables.begin();
    }

    typename std::vector<std::shared_ptr<T>>::iterator end() {
      return this->renderables.end();
    }

  private:

    RenderableContainer() {
      
    }

    void setRenderer(std::shared_ptr<ObjectRenderer<T>> renderer) {
      this->renderer = renderer;
    }


    std::vector<std::shared_ptr<T>> renderables;
  };
}
