#pragma once

#include <vector>
#include <memory>
#include <type_traits>

#include "../util/PersistentWeakPointer.h"
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

    PersistentWeakPointer<Object3D> root;
  };

}
