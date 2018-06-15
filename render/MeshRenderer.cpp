#include "MeshRenderer.h"
#include "../common/gl.h"
#include "RenderableContainer.h"

namespace Core {

    MeshRenderer::MeshRenderer(std::weak_ptr<Material> material, std::weak_ptr<Object3D> owner) : ObjectRenderer<Mesh>(owner), material(material) {
    }

    void MeshRenderer::renderObject(std::shared_ptr<Camera> camera, std::shared_ptr<Mesh> mesh) {
        WeakPointer<Object3D> ownerPtr(owner);
        WeakPointer<Material> materialPtr(this->material);
        WeakPointer<Shader> shaderPtr(materialPtr->getShader());

        glUseProgram(shaderPtr->getProgram());

        this->checkAndSetShaderAttribute(mesh, StandardAttributes::Position, mesh->getVertexPositions());
        this->checkAndSetShaderAttribute(mesh, StandardAttributes::Color, mesh->getVertexColors());
        this->checkAndSetShaderAttribute(mesh, StandardAttributes::UV, mesh->getVertexUVs());

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
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBuffer()->getBufferID());
            glDrawElements(GL_TRIANGLES, mesh->getVertexCount(), GL_UNSIGNED_INT, (void *)(0));
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, 36);
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

    void MeshRenderer::checkAndSetShaderAttribute(std::shared_ptr<Mesh> mesh, StandardAttributes attribute,
                                                  AttributeArrayBase* array) {
        if (mesh->isAttributeEnabled(attribute)) {
            WeakPointer<Material> materialPtr(this->material);
            GLuint shaderLocation = materialPtr->getShaderLocation(attribute);

            WeakPointer<AttributeArrayGPUStorage> gpuStoragePtr(array->getGPUStorage());
            if (gpuStoragePtr.isInitialized()) {
                gpuStoragePtr->sendToShader(shaderLocation);
            }
        }
    }
}