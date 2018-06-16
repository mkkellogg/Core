#pragma once

#include <memory>

#include "BaseObjectRenderer.h"

namespace Core  {

  // forward declarations
  class Object3D;
  class Graphics;

  template <typename T>
  class ObjectRenderer : public BaseObjectRenderer {
  public:
    ObjectRenderer(std::weak_ptr<Graphics> graphics, std::weak_ptr<Object3D> owner): BaseObjectRenderer(owner), graphics(graphics){}
    virtual void renderObject(std::weak_ptr<Camera> camera, std::weak_ptr<T> renderable) = 0;
  protected:
    std::weak_ptr<Graphics> graphics;
  };
}
