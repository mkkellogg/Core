#pragma once

#include "../common/gl.h"
#include "../common/debug.h"
#include "../scene/Scene.h"
#include "../scene/Object3D.h"
#include "../scene/Transform.h"
#include "../geometry/Vector2.h"
#include "../geometry/Vector4.h"
#include "Camera.h"
#include "ObjectRenderers.h"
#include "ObjectRenderer.h"

namespace Core {

  class Renderer {
  public:
    virtual ~Renderer();

    virtual Bool init();
    virtual void render(std::shared_ptr<Scene> scene) = 0;
    virtual void render(std::shared_ptr<Scene> scene, 
                        std::shared_ptr<Camera> camera,
                        std::vector<std::shared_ptr<Object3D>>& objectList) = 0;
    void processScene(std::shared_ptr<Scene> scene, 
                      std::vector<std::shared_ptr<Object3D>>& outObjects,
                      std::vector<std::shared_ptr<Camera>>& outCamerast);

    void setRenderSize(UInt32 width, UInt32 height,Bool updateViewport = true);    
    void setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);
    virtual void setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);
    Vector4u getViewport();
    
  private:
    void processSceneStep(const std::shared_ptr<Object3D> object,
                          std::vector<Transform*>& transformStack,
                          std::vector<std::shared_ptr<Object3D>>& outObjects,
                          std::vector<std::shared_ptr<Camera>>& outCameras);
  protected:
    Renderer();

    Vector2u renderSize;
    Vector4u viewport;

  };
}
