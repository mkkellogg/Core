#pragma once

#include <memory>
#include "../geometry/AttributeArray.h"
#include "../render/ObjectRenderer.h"
#include "../geometry/Mesh.h"
#include "../material/Material.h"
#include "../render/Camera.h"

namespace Core  {

  // forward declarations
  class Object3D;

  class MeshRenderer : public ObjectRenderer<Mesh>{
  public:
    MeshRenderer(std::shared_ptr<Material> material, std::weak_ptr<Object3D> owner);
    virtual void render(std::shared_ptr<Camera> camera) override;
    virtual void renderObject(std::shared_ptr<Camera> camera, std::shared_ptr<Mesh> mesh) override;

  private:
    std::shared_ptr<Material> material;
    template <typename T> void checkAndSetShaderAttribute(std::shared_ptr<Mesh> mesh, StandardAttributes attribute,
                                                          AttributeArray<T>* array, GLenum type, GLboolean normalize,
                                                          GLsizei stride, const GLvoid * pointer);
  };

  template <typename T> void MeshRenderer::checkAndSetShaderAttribute(std::shared_ptr<Mesh> mesh, StandardAttributes attribute,
                                                    AttributeArray<T>* array, GLenum type, GLboolean normalize,
                                                    GLsizei stride, const GLvoid * pointer) {
    if (mesh->isAttributeEnabled(attribute)) {
      GLuint shaderLocation = this->material->getShaderLocation(attribute);
      if (shaderLocation >= 0) {
        glBindBuffer(GL_ARRAY_BUFFER, array->getGLBufferID());
        glVertexAttribPointer(shaderLocation, array->getComponentCount(), type, normalize, stride, pointer);
        glEnableVertexAttribArray(shaderLocation);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
      }
    }
  }
}
