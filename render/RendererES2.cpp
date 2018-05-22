#include "RendererES2.h"
#include "../render/MeshRenderer.h"

namespace Core {

  RendererES2::RendererES2() {

  }

  Bool RendererES2::init() {
      return true;
  }

  RendererES2::~RendererES2() {

  }

  Renderer *RendererES2::createRenderer() {
    RendererES2 *renderer = new RendererES2;
    if (!renderer->init()) {
      delete renderer;
      return nullptr;
    }
    return renderer;
  }

  void RendererES2::render(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::vector<std::shared_ptr<Object3D>> objectList;
    this->processScene(scene, objectList);
    for (auto object : objectList) {
      /*std::shared_ptr<ObjectRenderer<Mesh>> renderer = object->getCustomRenderer<Mesh>();
      if (renderer) {
        renderer->render(camera);
      }*/
      for (auto itr = object->beginIterateRenderers(); itr != object->endIterateRenderers(); ++itr) {
        (*itr)->render(camera);
      }
    }
  }

}
