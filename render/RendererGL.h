#pragma once

#include "Renderer.h"
#include "../geometry/Mesh.h"

namespace Core {
  class RendererGL: public Renderer {
  public:
    RendererGL();
    virtual ~RendererGL();
    virtual Bool init() override;
    virtual void render(std::shared_ptr<Scene> scene) override;
    virtual void render(std::shared_ptr<Scene> scene, 
                        std::shared_ptr<Camera> camera,
                        std::vector<std::shared_ptr<Object3D>>& objectList) override;

    static Renderer* createRenderer();
  };
}
