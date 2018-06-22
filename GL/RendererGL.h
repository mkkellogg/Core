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
    void render(WeakPointer<Scene> scene) override;
    void render(WeakPointer<Scene> scene, WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objectList) override;
    void setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) override;

  private:
    RendererGL();
    static RendererGL* createRenderer();
  };
}
