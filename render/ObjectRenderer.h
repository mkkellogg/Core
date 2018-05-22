//
// Created by Mark Kellogg on 12/15/17.
//

#ifndef MPM_OBJECT_RENDERER_H
#define MPM_OBJECT_RENDERER_H

#include <memory>
#include "BaseObjectRenderer.h"

namespace Core  {

  // forward declarations
  class Object3D;

  template <typename T>
  class ObjectRenderer : public BaseObjectRenderer{

  public:

    ObjectRenderer(std::shared_ptr<Object3D> owner): BaseObjectRenderer(owner){}
    virtual void renderObject(std::shared_ptr<Camera> camera, std::shared_ptr<T> renderable) = 0;
  };
}



#endif //MPM_OBJECT_RENDERER_H
