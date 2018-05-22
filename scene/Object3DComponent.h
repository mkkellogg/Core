//
// Created by Mark Kellogg on 12/15/17.
//

#ifndef MPM_OBJECT_COMPONENT_H
#define MPM_OBJECT_COMPONENT_H

#include <memory>

namespace Core {

  // forward declaration
  class Object3D;

  class Object3DComponent {
  protected:
    std::shared_ptr<Object3D> owner;
  public:
    Object3DComponent(std::shared_ptr<Object3D> owner);
  };
}

#endif //MPM_OBJECT_COMPONENT_H
