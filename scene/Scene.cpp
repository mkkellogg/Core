#include "Scene.h"
#include "../common/assert.h"

namespace Core {

  Scene::Scene() {
    std::shared_ptr<Object3D> root = std::make_shared<Object3D>();
    this->root = root;
  }

  std::weak_ptr<Object3D> Scene::getRoot() {
    return this->root;
  }

}
