#include "MeshRenderer.h"
#include "RenderableContainer.h"
#include "../render/Camera.h"
#include "../material/Material.h"
#include "../geometry/AttributeArrayGPUStorage.h"
#include "../geometry/AttributeArray.h"
#include "../geometry/Mesh.h"

namespace Core {

    MeshRenderer::MeshRenderer(WeakPointer<Graphics> graphics, 
                               WeakPointer<Material> material, 
                               WeakPointer<Object3D> owner): ObjectRenderer<Mesh>(graphics, owner), material(material) {
    }

    void MeshRenderer::renderObject(WeakPointer<Camera> camera, WeakPointer<Mesh> mesh) {
        WeakPointer<Shader> shader = this->material->getShader();

        this->graphics->activateShader(this->material->getShader());

        this->checkAndSetShaderAttribute(mesh, StandardAttribute::Position, mesh->getVertexPositions());
        this->checkAndSetShaderAttribute(mesh, StandardAttribute::Color, mesh->getVertexColors());
        this->checkAndSetShaderAttribute(mesh, StandardAttribute::UV0, mesh->getVertexUVs());

        Int32 projectionLoc = this->material->getShaderLocation(StandardUniform::ProjectionMatrix);
        Int32 viewMatrixLoc = this->material->getShaderLocation(StandardUniform::ViewMatrix);
        Int32 modelMatrixLoc = this->material->getShaderLocation(StandardUniform::ModelMatrix);

        if (projectionLoc >= 0) {
            const Matrix4x4 &projMatrix = camera->getProjectionMatrix();
            shader->setUniformMatrix4(projectionLoc, projMatrix);
        }

        if (viewMatrixLoc >= 0) {;
            Matrix4x4 viewMatrix = camera->getOwner()->getTransform().getWorldMatrix();
            viewMatrix.invert();
            shader->setUniformMatrix4(viewMatrixLoc, viewMatrix);
        }

        if (modelMatrixLoc >= 0) {
            Matrix4x4 modelmatrix = this->owner->getTransform().getWorldMatrix();
            shader->setUniformMatrix4(modelMatrixLoc, modelmatrix);
        }

        this->material->sendCustomUniformsToShader();

        if (mesh->isIndexed()) {
            this->graphics->drawBoundVertexBuffer(mesh->getVertexCount(), mesh->getIndexBuffer());
        } else {
            this->graphics->drawBoundVertexBuffer(mesh->getVertexCount());
        }
    }

    void MeshRenderer::render(WeakPointer<Camera> camera) {
        std::shared_ptr<RenderableContainer<Mesh>> thisContainer = std::dynamic_pointer_cast<RenderableContainer<Mesh>>(this->owner.lock());
        if (thisContainer) {
            auto renderables = thisContainer->getRenderables();
            for (auto mesh : renderables) {
                this->renderObject(camera, mesh);
            }
        }
    }

    void MeshRenderer::checkAndSetShaderAttribute(WeakPointer<Mesh> mesh, StandardAttribute attribute, AttributeArrayBase *array) {
        if (mesh->isAttributeEnabled(attribute)) {
            Int32 shaderLocation = this->material->getShaderLocation(attribute);
            if (array->getGPUStorage()) {
                array->getGPUStorage()->sendToShader(shaderLocation);
            }
        }
    }
}