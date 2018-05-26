#pragma once

#include "BaseObjectRenderer.h"
#include "../scene/Object3D.h"

namespace Core {

  class BaseRenderableContainer : public Object3D {

  protected:
    std::shared_ptr<BaseObjectRenderer> renderer;
  public:

    std::shared_ptr<BaseObjectRenderer> getBaseRenderer() {
      return renderer;
    }
  };

}
