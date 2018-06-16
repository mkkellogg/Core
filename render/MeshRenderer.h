#pragma once

#include <memory>

#include "../util/WeakPointer.h"
#include "../geometry/AttributeArray.h"
#include "../geometry/AttributeArrayGPUStorage.h"
#include "../render/ObjectRenderer.h"
#include "../geometry/Mesh.h"
#include "../material/Material.h"
#include "../render/Camera.h"

namespace Core  {

  // forward declarations
  class Object3D;
  class Engine;

  class MeshRenderer : public ObjectRenderer<Mesh>{
    friend class Engine;

  public:
    virtual void render(WeakPointer<Camera> camera) override;
    virtual void renderObject(WeakPointer<Camera> camera, WeakPointer<Mesh> mesh) override;

  private:
    MeshRenderer(WeakPointer<Graphics> graphics, WeakPointer<Material> material, WeakPointer<Object3D> owner);
    void checkAndSetShaderAttribute(WeakPointer<Mesh> mesh, StandardAttributes attribute, AttributeArrayBase* array);

    WeakPointer<Material> material;
  };

}
