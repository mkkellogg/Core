//
// Created by Mark Kellogg on 12/14/17.
//

#include "Scene.h"
#include "../common/assert.h"

namespace MPM {

  Scene::Scene() {
    std::shared_ptr<Object3D> root = std::make_shared<Object3D>();
    this->root = root;
  }

  std::shared_ptr<Object3D> Scene::getRoot() {
    return this->root;
  }

}