#include "Scene.h"
#include "../common/assert.h"

namespace Core {

  Scene::Scene(std::weak_ptr<Object3D> root): root(root) {
    
  }

  std::weak_ptr<Object3D> Scene::getRoot() {
    return this->root;
  }

}
