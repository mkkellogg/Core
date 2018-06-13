#include "MeshRenderer.h"
#include "../common/gl.h"
#include "RenderableContainer.h"
#include "../GL/MeshGL.h"

namespace Core {

  MeshRenderer::MeshRenderer(std::weak_ptr<Material> material, std::weak_ptr<Object3D> owner):
      ObjectRenderer<Mesh>(owner), material(material) {

  }

  void MeshRenderer::renderObject(std::shared_ptr<Camera> camera, std::shared_ptr<Mesh> mesh) {
    WeakPointer<Object3D> ownerPtr(owner);
    WeakPointer<Material> materialPtr(this->material);

    glUseProgram(materialPtr->getShader()->getProgram());

    this->checkAndSetShaderAttribute(mesh, StandardAttributes::Position, mesh->getVertexPositions(), GL_FLOAT, GL_FALSE, 0, 0);
    this->checkAndSetShaderAttribute(mesh, StandardAttributes::Color, mesh->getVertexColors(), GL_FLOAT, GL_FALSE, 0, 0);
    this->checkAndSetShaderAttribute(mesh, StandardAttributes::UV, mesh->getVertexUVs(), GL_FLOAT, GL_FALSE, 0, 0);

    GLint projectionLoc = materialPtr->getShaderLocation(StandardUniforms::ProjectionMatrix);
    GLint viewMatrixLoc = materialPtr->getShaderLocation(StandardUniforms::ViewMatrix);
    GLint modelMatrixLoc = materialPtr->getShaderLocation(StandardUniforms::ModelMatrix);

    if (projectionLoc >= 0) {
      const Matrix4x4 &matrix = camera->getProjectionMatrix();
      glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, matrix.getConstData());
    }

    if (viewMatrixLoc >= 0) {
      Matrix4x4 viewMatrix = camera->getTransform().getWorldMatrix();
      viewMatrix.invert();
      glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, viewMatrix.getConstData());
    }

     if (modelMatrixLoc >= 0) {
      Matrix4x4 modelmatrix = ownerPtr->getTransform().getWorldMatrix();
      glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelmatrix.getConstData());
    }

    materialPtr->sendCustomUniformsToShader();

    if (mesh->isIndexed()) {
       std::shared_ptr<MeshGL> meshGL = std::dynamic_pointer_cast<MeshGL>(mesh);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshGL->getIndexBuffer());
      glDrawElements(GL_TRIANGLES, mesh->getSize(), GL_UNSIGNED_INT, (void *) (0));
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    } else {
      glDrawArrays(GL_TRIANGLES, 0, mesh->getSize());
    }
  }

  void MeshRenderer::render(std::shared_ptr<Camera> camera) {
    WeakPointer<Object3D> ownerPtr(owner);
    std::shared_ptr<RenderableContainer<Mesh>> thisContainer = std::dynamic_pointer_cast<RenderableContainer<Mesh>>(ownerPtr.getLockedPointer());
    if (thisContainer) {
      auto renderables = thisContainer->getRenderables();
      for (auto mesh : renderables) {
        WeakPointer<Mesh> meshPtr(mesh);
        this->renderObject(camera, meshPtr.getLockedPointer());
      }
    }
  }
}