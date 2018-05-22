//
// Created by mark on 12/13/17.
//

#ifndef MPM_RENDERERES2_H
#define MPM_RENDERERES2_H

#include "Renderer.h"
#include "../geometry/Mesh.h"

namespace Core {
  class RendererES2: public Renderer {
  public:
    RendererES2();
    virtual ~RendererES2();
    Bool init();
    virtual void render(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera) override;

  private:

  public:
    static Renderer* createRenderer();
  };
}

#endif //MPM_RENDERERES2_H
