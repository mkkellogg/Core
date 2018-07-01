#include "MeshRenderer.h"
#include "RenderableContainer.h"
#include "../render/Camera.h"
#include "../material/Material.h"
#include "../material/Shader.h"
#include "../geometry/AttributeArrayGPUStorage.h"
#include "../geometry/AttributeArray.h"
#include "../geometry/Mesh.h"
#include "../light/PointLight.h"
#include "../Engine.h"

namespace Core {

    MeshRenderer::MeshRenderer(WeakPointer<Graphics> graphics, 
                               WeakPointer<Material> material, 
                               WeakPointer<Object3D> owner): ObjectRenderer<Mesh>(graphics, owner), material(material) {
    }

    void MeshRenderer::renderObject(WeakPointer<Camera> camera, WeakPointer<Mesh> mesh) {
        static WeakPointer<PointLight> testLight = Engine::instance()->createLight<PointLight>(this->getOwner());
        testLight->setColor(1.0f, 0.6f, 0.0f, 1.0f);
        testLight->setRadius(10.0f);
        testLight->setPosition(10.0f, 20.0f, -10.0f);

        WeakPointer<Shader> shader = this->material->getShader();
        this->graphics->activateShader(shader);

        // send custom uniforms first so that the renderer can override if necessary.
        this->material->sendCustomUniformsToShader();

        this->checkAndSetShaderAttribute(mesh, StandardAttribute::Position, mesh->getVertexPositions());
        this->checkAndSetShaderAttribute(mesh, StandardAttribute::Normal, mesh->getVertexNormals());
        this->checkAndSetShaderAttribute(mesh, StandardAttribute::Color, mesh->getVertexColors());
        this->checkAndSetShaderAttribute(mesh, StandardAttribute::UV0, mesh->getVertexUVs0());

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

        Int32 lightPositionLoc = this->material->getShaderLocation(StandardUniform::LightPosition);
        Int32 lightRangeLoc = this->material->getShaderLocation(StandardUniform::LightRange);
        Int32 lightTypeLoc = this->material->getShaderLocation(StandardUniform::LightType);
        Int32 lightIntensityLoc = this->material->getShaderLocation(StandardUniform::LightIntensity);
        Int32 lightColorLoc = this->material->getShaderLocation(StandardUniform::LightColor);

        if (lightPositionLoc >= 0) {
            Point3r pos = testLight->getPosition();
            shader->setUniform4f(lightPositionLoc, pos.x, pos.y, pos.z, 1.0f);
        }

        if (lightRangeLoc >= 0) {
            shader->setUniform1f(lightRangeLoc, testLight->getRadius());
        }

       if (lightTypeLoc >= 0) {
            shader->setUniform1i(lightRangeLoc, (Int32)LightType::Point);
        }

        if (lightIntensityLoc >= 0) {
            shader->setUniform1f(lightIntensityLoc, testLight->getIntensity());
        }

        if (lightColorLoc >= 0) {
            Color color = testLight->getColor();
            shader->setUniform4f(lightPositionLoc, color.r, color.g, color.b, color.a);
        }

        if (mesh->isIndexed()) {
            this->graphics->drawBoundVertexBuffer(mesh->getIndexCount(), mesh->getIndexBuffer());
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

    void MeshRenderer::checkAndSetShaderAttribute(WeakPointer<Mesh> mesh, StandardAttribute attribute, WeakPointer<AttributeArrayBase> array) {
        if (mesh->isAttributeEnabled(attribute)) {
            Int32 shaderLocation = this->material->getShaderLocation(attribute);
            if (array->getGPUStorage()) {
                array->getGPUStorage()->sendToShader(shaderLocation);
            }
        }
    }
}