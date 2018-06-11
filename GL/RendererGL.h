#pragma once

#include "../common/gl.h"
#include "Renderer.h"
#include "../geometry/Mesh.h"

namespace Core {

  // forward declaration
  class Engine;

  class RendererGL: public Renderer {
    friend class Engine;

  public:

    virtual ~RendererGL();
    virtual Bool init() override;
    virtual void render(std::weak_ptr<Scene> scene) override;
    virtual void render(std::weak_ptr<Scene> scene, 
                        std::shared_ptr<Camera> camera,
                        std::vector<std::shared_ptr<Object3D>>& objectList) override;

    
    virtual void setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) override;

  private:
    RendererGL();
    static Renderer* createRenderer();
  };
}
