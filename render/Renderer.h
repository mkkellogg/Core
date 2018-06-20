#pragma once

#include "../common/complextypes.h"
#include "../util/WeakPointer.h"
#include "../scene/Object3DComponent.h"
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
    virtual void render(WeakPointer<Scene> scene) = 0;
    virtual void render(WeakPointer<Scene> scene, 
                        WeakPointer<Camera> camera,
                        std::vector<WeakPointer<Object3D>>& objectList) = 0;
    void processScene(WeakPointer<Scene> scene, 
                      std::vector<WeakPointer<Object3D>>& outObjects,
                      std::vector<WeakPointer<Camera>>& outCamerast);

    void setRenderSize(UInt32 width, UInt32 height,Bool updateViewport = true);    
    void setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);
    virtual void setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);
    Vector4u getViewport();
    
  private:
    void processSceneStep(WeakPointer<Object3D> object,
                          const Matrix4x4& curTransform,
                          std::vector<WeakPointer<Object3D>>& outObjects,
                          std::vector<WeakPointer<Camera>>& outCameras);
  protected:
    Renderer();

    Vector2u renderSize;
    Vector4u viewport;

  };
}
