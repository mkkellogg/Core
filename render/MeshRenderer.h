#pragma once

#include <memory>
#include "../geometry/AttributeArray.h"
#include "../render/ObjectRenderer.h"
#include "../geometry/Mesh.h"
#include "../material/Material.h"
#include "../render/Camera.h"
#include "../util/WeakPointer.h"
#include "../common/gl.h"

namespace Core  {

  // forward declarations
  class Object3D;
  class Engine;

  class MeshRenderer : public ObjectRenderer<Mesh>{
    friend class Engine;

  public:
    virtual void render(std::shared_ptr<Camera> camera) override;
    virtual void renderObject(std::shared_ptr<Camera> camera, std::shared_ptr<Mesh> mesh) override;

  private:
    MeshRenderer(std::weak_ptr<Material> material, std::weak_ptr<Object3D> owner);
    std::weak_ptr<Material> material;
    template <typename T> void checkAndSetShaderAttribute(std::shared_ptr<Mesh> mesh, StandardAttributes attribute, AttributeArray<T>* array);
  };

  template <typename T> void MeshRenderer::checkAndSetShaderAttribute(std::shared_ptr<Mesh> mesh, StandardAttributes attribute, AttributeArray<T>* array) {
    if (mesh->isAttributeEnabled(attribute)) {
      WeakPointer<Material> materialPtr(this->material);
      GLuint shaderLocation = materialPtr->getShaderLocation(attribute);
      WeakPointer<AttributeArrayGPUStorage> gpuBuffer(array->getGPUStorage());

      if (gpuBuffer.isInitialized()) {
        gpuBuffer->sendToShader(shaderLocation);
      }
    }
  }

}
