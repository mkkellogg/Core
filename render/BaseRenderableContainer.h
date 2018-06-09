#pragma once

#include "BaseObjectRenderer.h"
#include "../scene/Object3D.h"

namespace Core {

  class BaseRenderableContainer : public Object3D {
  public:

    virtual ~BaseRenderableContainer() = 0;
    std::shared_ptr<BaseObjectRenderer> getBaseRenderer() {
      return renderer;
    }

  protected:
    std::shared_ptr<BaseObjectRenderer> renderer;
  };

}
