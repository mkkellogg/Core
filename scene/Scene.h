#pragma once

#include <vector>
#include <memory>

#include "../common/types.h"
#include "Object3D.h"

namespace Core {

  class Scene {
  public:
    Scene();
    std::shared_ptr<Object3D> getRoot();

  private:
    std::shared_ptr<Object3D> root;
  };

}
