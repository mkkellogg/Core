#pragma once

#include <vector>
#include <memory>
#include <type_traits>

#include "../common/types.h"
#include "Object3D.h"

namespace Core {

  //forward declarations
  class Engine;

  class Scene {
    friend class Engine;

  public:
    WeakPointer<Object3D> getRoot();

  private:
    Scene(WeakPointer<Object3D> root);

    WeakPointer<Object3D> root;
  };

}
