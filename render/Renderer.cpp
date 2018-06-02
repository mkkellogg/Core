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

  void Renderer::processScene(std::shared_ptr<Scene> scene, 
                              std::vector<std::shared_ptr<Object3D>>& outObjects,
                              std::vector<std::shared_ptr<Camera>>& outCameras) {
    std::vector<Transform*> transformStack;
    processSceneStep(scene->getRoot(), transformStack, outObjects, outCameras);
  }

  void Renderer::processSceneStep(const std::shared_ptr<Object3D> object,
                                  std::vector<Transform*>& transformStack,
                                  std::vector<std::shared_ptr<Object3D>>& outObjects,
                                  std::vector<std::shared_ptr<Camera>>& outCameras) {
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
      outObjects.push_back(obj);

      // check if this object is a camera
      std::shared_ptr<Camera> cam = std::dynamic_pointer_cast<Camera>(obj);
      if (cam) {
        outCameras.push_back(cam);
      }

      this->processSceneStep(obj, transformStack, outObjects, outCameras);
      transformStack.pop_back();
    }
  }

  void Renderer::setRenderSize(UInt32 width, UInt32 height, Bool updateViewport) {
    this->renderSize.x = width;
    this->renderSize.y = height;
    if (updateViewport) {
      this->setViewport(0, 0, width, height);
    }
  }

  void Renderer::setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
    this->renderSize.x = width;
    this->renderSize.y = height;
    this->setViewport(hOffset, vOffset, viewPortWidth, viewPortHeight);
  }

  void Renderer::setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
    this->viewport.set(hOffset, vOffset, viewPortWidth, viewPortHeight);
  }

  Vector4u Renderer::getViewport() {
    return this->viewport;
  }
}
