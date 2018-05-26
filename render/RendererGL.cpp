#include "RendererGL.h"
#include "../render/MeshRenderer.h"
#include "../render/RenderableContainer.h"
#include "../render/BaseRenderableContainer.h"

namespace Core {

  RendererGL::RendererGL() {

  }

  Bool RendererGL::init() {
      Renderer::init();
      return true;
  }

  RendererGL::~RendererGL() {

  }

  Renderer* RendererGL::createRenderer() {
    RendererGL *renderer = new RendererGL;
    if (!renderer->init()) {
      delete renderer;
      return nullptr;
    }
    return renderer;
  }

  void RendererGL::render(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::vector<std::shared_ptr<Object3D>> objectList;
    this->processScene(scene, objectList);
    for (auto object : objectList) {
      std::shared_ptr<BaseRenderableContainer> renderableContainer = std::dynamic_pointer_cast<BaseRenderableContainer>(object);
      if(renderableContainer) {
        std::shared_ptr<BaseObjectRenderer> baseRenderer = renderableContainer->getBaseRenderer();
        if(baseRenderer) {
          baseRenderer->render(camera);
        }
      }
    }
  }

}
