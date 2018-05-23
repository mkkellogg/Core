#pragma once

#include "Renderer.h"
#include "../geometry/Mesh.h"

namespace Core {
  class RendererGL: public Renderer {
  public:
    RendererGL();
    virtual ~RendererGL();
    Bool init();
    virtual void render(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera) override;

  private:

  public:
    static Renderer* createRenderer();
  };
}
