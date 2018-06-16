#include "MeshRenderer.h"
#include "RenderableContainer.h"

namespace Core {

    MeshRenderer::MeshRenderer(std::weak_ptr<Graphics> graphics, 
                               std::weak_ptr<Material> material, 
                               std::weak_ptr<Object3D> owner): ObjectRenderer<Mesh>(graphics, owner), material(material) {
    }

    void MeshRenderer::renderObject(std::weak_ptr<Camera> camera, std::weak_ptr<Mesh> mesh) {
        WeakPointer<Object3D> ownerPtr(owner);
        WeakPointer<Material> materialPtr(this->material);
        WeakPointer<Shader> shaderPtr(materialPtr->getShader());
        WeakPointer<Graphics> graphicsPtr(this->graphics);
        WeakPointer<Mesh> meshPtr(mesh);
        WeakPointer<Camera> cameraPtr(camera);

        graphicsPtr->activateShader(materialPtr->getShader());

        this->checkAndSetShaderAttribute(meshPtr, StandardAttributes::Position, meshPtr->getVertexPositions());
        this->checkAndSetShaderAttribute(meshPtr, StandardAttributes::Color, meshPtr->getVertexColors());
        this->checkAndSetShaderAttribute(meshPtr, StandardAttributes::UV, meshPtr->getVertexUVs());

        Int32 projectionLoc = materialPtr->getShaderLocation(StandardUniforms::ProjectionMatrix);
        Int32 viewMatrixLoc = materialPtr->getShaderLocation(StandardUniforms::ViewMatrix);
        Int32 modelMatrixLoc = materialPtr->getShaderLocation(StandardUniforms::ModelMatrix);

        if (projectionLoc >= 0) {
            const Matrix4x4 &projMatrix = cameraPtr->getProjectionMatrix();
            shaderPtr->setUniformMatrix4(projectionLoc, projMatrix);
        }

        if (viewMatrixLoc >= 0) {
            WeakPointer<Object3D> cameraOwner(cameraPtr->getOwner());
            Matrix4x4 viewMatrix = cameraOwner->getTransform().getWorldMatrix();
            viewMatrix.invert();
            shaderPtr->setUniformMatrix4(viewMatrixLoc, viewMatrix);
        }

        if (modelMatrixLoc >= 0) {
            Matrix4x4 modelmatrix = ownerPtr->getTransform().getWorldMatrix();
            shaderPtr->setUniformMatrix4(modelMatrixLoc, modelmatrix);
        }

        materialPtr->sendCustomUniformsToShader();

        if (meshPtr->isIndexed()) {
            graphicsPtr->drawBoundVertexBuffer(meshPtr->getVertexCount(), meshPtr->getIndexBuffer());
        } else {
            graphicsPtr->drawBoundVertexBuffer(meshPtr->getVertexCount());
        }
    }

    void MeshRenderer::render(std::weak_ptr<Camera> camera) {
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

    void MeshRenderer::checkAndSetShaderAttribute(WeakPointer<Mesh> mesh, StandardAttributes attribute, AttributeArrayBase *array) {
        if (mesh->isAttributeEnabled(attribute)) {
            WeakPointer<Material> materialPtr(this->material);
            Int32 shaderLocation = materialPtr->getShaderLocation(attribute);

            WeakPointer<AttributeArrayGPUStorage> gpuStoragePtr(array->getGPUStorage());
            if (gpuStoragePtr) {
                gpuStoragePtr->sendToShader(shaderLocation);
            }
        }
    }
}