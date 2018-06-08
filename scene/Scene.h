#pragma once

#include <vector>
#include <memory>

#include "../common/types.h"
#include "Object3D.h"

namespace Core {

  //forward declarations
  class Engine;

  class Scene {
    friend class Engine;

  public:
    std::weak_ptr<Object3D> getRoot();

  private:
    Scene();

    std::shared_ptr<Object3D> root;
  };

}
