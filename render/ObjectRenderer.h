#pragma once

#include <memory>
#include "BaseObjectRenderer.h"

namespace Core  {

  // forward declarations
  class Object3D;

  template <typename T>
  class ObjectRenderer : public BaseObjectRenderer {
  public:
    ObjectRenderer(std::weak_ptr<Object3D> owner): BaseObjectRenderer(owner){}
    virtual void renderObject(std::shared_ptr<Camera> camera, std::shared_ptr<T> renderable) = 0;
  };
}
