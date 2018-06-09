#include "Scene.h"
#include "../common/assert.h"

namespace Core {

  Scene::Scene() {
    this->root = std::shared_ptr<Object3D>(new Object3D());
  }

  std::weak_ptr<Object3D> Scene::getRoot() {
    return this->root;
  }

}
