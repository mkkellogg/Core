//
// Created by Mark Kellogg on 12/29/17.
//

#ifndef MPM_RENDERABLE_GROUP_H
#define MPM_RENDERABLE_GROUP_H

#include <memory>
#include <vector>
#include "../scene/Object3DComponent.h"

namespace MPM {

  template <typename T>
  class RenderableGroup : Object3DComponent {
    std::vector<std::shared_ptr<T>> renderables;
  public:
    RenderableGroup(std::shared_ptr<Object3D> owner) : Object3DComponent(owner) {}


    void addMesh(std::shared_ptr<T> mesh) {
      this->renderables.push_back(mesh);
    }

    void clearMeshes() {
      this->renderables.clear();
    }

    std::vector<std::shared_ptr<T>>::iterator begin() {
      return this->renderables.begin();
    }

    std::vector<std::shared_ptr<T>>::iterator end() {
      return this->renderables.end();
    }

  };

}

#endif //MPM_RENDERABLE_GROUP_H
