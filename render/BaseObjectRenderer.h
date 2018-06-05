#pragma once

#include <memory>
#include "../scene/Object3DComponent.h"
#include "BaseRenderable.h"

namespace Core  {

  // forward declarations
  class Object3D;
  class Camera;

  class BaseObjectRenderer : public Object3DComponent{
  public:

    BaseObjectRenderer(std::shared_ptr<Object3D> owner): Object3DComponent(owner){}
    virtual void render(std::shared_ptr<Camera> camera) = 0;
  };
}
