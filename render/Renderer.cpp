//
// Created by Mark Kellogg on 12/13/17.
//

#include "Renderer.h"
#include "../math/Matrix4x4.h"

namespace Core {

  Renderer::Renderer() {

  }

  Renderer::~Renderer() {

  }

  Bool Renderer::init() {
    return true;
  }

  void Renderer::processScene(std::shared_ptr<Scene> scene, std::vector<std::shared_ptr<Object3D>>& out) {
    std::vector<Transform> transformStack;
    transformStack.push_back(Transform());
    processSceneStep(scene->getRoot(), transformStack, out);
  }

  void Renderer::processSceneStep(const std::shared_ptr<Object3D> object,
                                  std::vector<Transform>& transformStack,
                                  std::vector<std::shared_ptr<Object3D>>& out) {
    for (auto itr = object->beginIterateChildren(); itr != object->endIterateChildren(); ++itr) {
      const std::shared_ptr<Object3D> obj = *itr;
      Transform nextTransform(transformStack.back());
      nextTransform.transformBy(obj->getLocalTransform());
      transformStack.push_back(nextTransform);
      obj->getWorldTransform().copy(nextTransform);
      out.push_back(obj);
      this->processSceneStep(obj, transformStack, out);
      transformStack.pop_back();
    }
  }

}
