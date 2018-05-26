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
    std::vector<Transform*> transformStack;
    processSceneStep(scene->getRoot(), transformStack, out);
  }

  void Renderer::processSceneStep(const std::shared_ptr<Object3D> object,
                                  std::vector<Transform*>& transformStack,
                                  std::vector<std::shared_ptr<Object3D>>& out) {
    for (auto itr = object->beginIterateChildren(); itr != object->endIterateChildren(); ++itr) {
      const std::shared_ptr<Object3D> obj = *itr;

      Matrix4x4 nextMatrix;
      if (transformStack.size() > 0) {
        nextMatrix = transformStack.back()->getWorldMatrix();
      }

      Transform& objTransform = obj->getTransform();
      nextMatrix.multiply(objTransform.getLocalMatrix());
      objTransform.getWorldMatrix().copy(nextMatrix);

      transformStack.push_back(&objTransform);
      out.push_back(obj);
      this->processSceneStep(obj, transformStack, out);
      transformStack.pop_back();
    }
  }

}
