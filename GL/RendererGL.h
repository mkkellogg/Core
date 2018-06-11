#pragma once

#include "../common/gl.h"
#include "../render/Renderer.h"
#include "../geometry/Mesh.h"

namespace Core {

  // forward declaration
  class GraphicsGL;

  class RendererGL final: public Renderer {
    friend class GraphicsGL;

  public:

    ~RendererGL();
    Bool init() override;
    void render(std::weak_ptr<Scene> scene) override;
    void render(std::weak_ptr<Scene> scene, std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<Object3D>>& objectList) override;
    virtual void setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) override;

  private:
    RendererGL();
    static RendererGL* createRenderer();
  };
}
