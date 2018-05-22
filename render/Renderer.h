#pragma once

#include "../common/gl.h"
#include "../common/debug.h"
#include "../scene/Scene.h"
#include "../scene/Object3D.h"
#include "../scene/Transform.h"
#include "Camera.h"
#include "ObjectRenderers.h"
#include "ObjectRenderer.h"

namespace Core {

  class Renderer {
    void processSceneStep(const std::shared_ptr<Object3D> object,
                          std::vector<Transform>& transformStack,
                          std::vector<std::shared_ptr<Object3D>>& out);

  public:
    virtual ~Renderer();

    void resize(int w, int h);
    virtual void render(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera) = 0;
    void processScene(std::shared_ptr<Scene> scene, std::vector<std::shared_ptr<Object3D>>& out);

  protected:
    Renderer();

  };
}
