#include "RendererGL.h"
#include "../render/MeshRenderer.h"
#include "../render/RenderableContainer.h"
#include "../render/BaseRenderableContainer.h"

#if !defined(DYNAMIC_ES3)
static GLboolean gl3stubInit() {
  return GL_TRUE;
}
#endif

namespace Core {

  RendererGL::RendererGL() {

  }

  Bool RendererGL::init() {
      Renderer::init();
      return true;
  }

  RendererGL::~RendererGL() {

  }

  RendererGL* RendererGL::createRenderer() {
    RendererGL *renderer = new RendererGL;
    if (!renderer->init()) {
      delete renderer;
      return nullptr;
    }
    return renderer;
  }

  void RendererGL::setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
    Renderer::setViewport(hOffset, vOffset, viewPortWidth, viewPortHeight);
    glViewport(hOffset, vOffset, viewPortWidth, viewPortHeight);
  }

  void RendererGL::render(std::weak_ptr<Scene> scene) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    std::vector<std::weak_ptr<Object3D>> objectList;
    std::vector<std::weak_ptr<Camera>> cameraList;
    this->processScene(scene, objectList, cameraList);
    for(auto camera: cameraList) {
      WeakPointer<Camera> cameraPtr(camera);
      cameraPtr->setAspectRatioFromDimensions(this->renderSize.x, this->renderSize.y);
      render(scene, camera, objectList);
    }
  }

  void RendererGL::render(std::weak_ptr<Scene> scene, 
                          std::weak_ptr<Camera> camera,
                          std::vector<std::weak_ptr<Object3D>>& objectList) {
    for (auto object : objectList) {
      WeakPointer<Object3D> objectPtr(object);

      if (objectPtr) {
        std::shared_ptr<Object3D> objectShared = objectPtr.getLockedPointer();
        std::shared_ptr<BaseRenderableContainer> containerPtr = std::dynamic_pointer_cast<BaseRenderableContainer>(objectShared);
        if(containerPtr) {
          std::weak_ptr<BaseObjectRenderer> objectRenderer = containerPtr->getBaseRenderer();
          WeakPointer<BaseObjectRenderer> objectRendererPtr(objectRenderer);
          if(objectRendererPtr) {
            objectRendererPtr->render(camera);
          }
        }
      }
    }
  }

}
