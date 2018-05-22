//
// Created by Mark Kellogg on 12/15/17.
//

#ifndef MPM_MESHRENDERER_H
#define MPM_MESHRENDERER_H

#include <memory>
#include "../geometry/AttributeArray.h"
#include "../render/ObjectRenderer.h"
#include "../geometry/Mesh.h"
#include "../material/Material.h"
#include "../render/Camera.h"

namespace MPM  {

  // forward declarations
  class Object3D;

  class MeshRenderer : public ObjectRenderer<Mesh>{
    std::shared_ptr<Material> material;
    template <typename T> void checkAndSetShaderAttribute(std::shared_ptr<Mesh> mesh, StandardAttributes attribute,
                                        AttributeArray<T>* array, GLenum type, GLboolean normalize,
                                        GLsizei stride, const GLvoid * pointer);

  public:
    MeshRenderer(std::shared_ptr<Material> material, std::shared_ptr<Object3D> owner);
    virtual void render(std::shared_ptr<Camera> camera) override;
    virtual void renderObject(std::shared_ptr<Camera> camera, std::shared_ptr<Mesh> mesh) override;
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



#endif //MPM_MESHRENDERER_H
