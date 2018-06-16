#pragma once

#include <memory>
#include "../geometry/AttributeArray.h"
#include "../geometry/AttributeArrayGPUStorage.h"
#include "../render/ObjectRenderer.h"
#include "../geometry/Mesh.h"
#include "../material/Material.h"
#include "../render/Camera.h"
#include "../util/WeakPointer.h"

namespace Core  {

  // forward declarations
  class Object3D;
  class Engine;

  class MeshRenderer : public ObjectRenderer<Mesh>{
    friend class Engine;

  public:
    virtual void render(std::weak_ptr<Camera> camera) override;
    virtual void renderObject(std::weak_ptr<Camera> camera, std::weak_ptr<Mesh> mesh) override;

  private:
    MeshRenderer(std::weak_ptr<Graphics> graphics, std::weak_ptr<Material> material, std::weak_ptr<Object3D> owner);
    void checkAndSetShaderAttribute(WeakPointer<Mesh> mesh, StandardAttributes attribute, AttributeArrayBase* array);

    std::weak_ptr<Material> material;
  };

}
