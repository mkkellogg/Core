//
// Created by Mark Kellogg on 12/15/17.
//

#ifndef MPM_BASE_OBJECT_RENDERER_H
#define MPM_BASE_OBJECT_RENDERER_H

#include <memory>
#include "../scene/Object3DComponent.h"
#include "BaseRenderable.h"

namespace MPM  {

  // forward declarations
  class Object3D;
  class Camera;

  class BaseObjectRenderer : public Object3DComponent{

  public:

    BaseObjectRenderer(std::shared_ptr<Object3D> owner): Object3DComponent(owner){}
    virtual void render(std::shared_ptr<Camera> camera) = 0;
  };
}



#endif //MPM_BASE_OBJECT_RENDERER_H
