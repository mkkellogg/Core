#include "MeshRenderer.h"
#include "RenderableContainer.h"
#include "../render/Camera.h"
#include "../render/RenderTarget.h"
#include "../material/Material.h"
#include "../material/Shader.h"
#include "../image/Texture.h"
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

    void MeshRenderer::renderObject(const ViewDescriptor& viewDescriptor, WeakPointer<Mesh> mesh, const std::vector<WeakPointer<Light>>& lights) {
       
        
        WeakPointer<Material> material;
        if (viewDescriptor.overrideMaterial.isValid()) {
            material = viewDescriptor.overrideMaterial;
        }
        else {
            material = this->material;
        }

        WeakPointer<Shader> shader = material->getShader();
        this->graphics->activateShader(shader);

        // send custom uniforms first so that the renderer can override if necessary.
        material->sendCustomUniformsToShader();

        this->checkAndSetShaderAttribute(mesh, material, StandardAttribute::Position, mesh->getVertexPositions());
        this->checkAndSetShaderAttribute(mesh, material, StandardAttribute::Normal, mesh->getVertexNormals());
        this->checkAndSetShaderAttribute(mesh, material, StandardAttribute::FaceNormal, mesh->getVertexFaceNormals());
        this->checkAndSetShaderAttribute(mesh, material, StandardAttribute::Color, mesh->getVertexColors());
        this->checkAndSetShaderAttribute(mesh, material, StandardAttribute::UV0, mesh->getVertexUVs0());

        Int32 projectionLoc = material->getShaderLocation(StandardUniform::ProjectionMatrix);
        Int32 viewMatrixLoc = material->getShaderLocation(StandardUniform::ViewMatrix);
        Int32 modelMatrixLoc = material->getShaderLocation(StandardUniform::ModelMatrix);
        Int32 modelInverseTransposeMatrixLoc = material->getShaderLocation(StandardUniform::ModelInverseTransposeMatrix);
        Int32 viewInverseTransposeMatrixLoc = material->getShaderLocation(StandardUniform::ViewInverseTransposeMatrix);

        if (projectionLoc >= 0) {
            const Matrix4x4 &projMatrix = viewDescriptor.projectionMatrix;
            shader->setUniformMatrix4(projectionLoc, projMatrix);
        }

        if (viewMatrixLoc >= 0) {;
            Matrix4x4 viewMatrix = viewDescriptor.viewInverseMatrix;
            shader->setUniformMatrix4(viewMatrixLoc, viewMatrix);
        }

        if (modelMatrixLoc >= 0) {
            Matrix4x4 modelmatrix = this->owner->getTransform().getWorldMatrix();
            shader->setUniformMatrix4(modelMatrixLoc, modelmatrix);
        }

        if (modelInverseTransposeMatrixLoc >= 0) {
            Matrix4x4 modelInverseTransposeMatrix = this->owner->getTransform().getWorldMatrix();
            modelInverseTransposeMatrix.invert();
            modelInverseTransposeMatrix.transpose();
            shader->setUniformMatrix4(modelInverseTransposeMatrixLoc, modelInverseTransposeMatrix);
        }

        if (viewInverseTransposeMatrixLoc >= 0) {
            Matrix4x4 viewInverseTransposeMatrix = viewDescriptor.viewInverseTransposeMatrix;
            shader->setUniformMatrix4(viewInverseTransposeMatrixLoc, viewInverseTransposeMatrix);
        }

        Int32 lightPositionLoc = material->getShaderLocation(StandardUniform::LightPosition);
        Int32 lightRangeLoc = material->getShaderLocation(StandardUniform::LightRange);
        Int32 lightTypeLoc = material->getShaderLocation(StandardUniform::LightType);
        Int32 lightIntensityLoc = material->getShaderLocation(StandardUniform::LightIntensity);
        Int32 lightColorLoc = material->getShaderLocation(StandardUniform::LightColor);
        Int32 lightEnabledLoc = material->getShaderLocation(StandardUniform::LightEnabled);

        Int32 lightMatrixLoc = material->getShaderLocation(StandardUniform::LightMatrix);
        Int32 lightShadowMapLoc = material->getShaderLocation(StandardUniform::LightShadowMap);
        Int32 lightShadowCubeMapLoc = material->getShaderLocation(StandardUniform::LightShadowCubeMap);
        Int32 lightShadowBiasLoc = material->getShaderLocation(StandardUniform::LightShadowBias);

        UInt32 currentTextureSlot = material->textureCount();

        if (lights.size() > 0) {
            if (lightEnabledLoc >= 0) {
                shader->setUniform1i(lightEnabledLoc, 1);
            }
            for(UInt32 i = 0; i < lights.size(); i++) {
                WeakPointer<Light> light = lights[i];
                LightType lightType = light->getType();
                if (lightColorLoc >= 0) {
                    Color color = light->getColor();
                    shader->setUniform4f(lightPositionLoc, color.r, color.g, color.b, color.a);
                }

                if (lightTypeLoc >= 0) {
                    shader->setUniform1i(lightTypeLoc, (Int32)lightType);
                }

                if (lightIntensityLoc >= 0) {
                    shader->setUniform1f(lightIntensityLoc, light->getIntensity());
                }

                if (lightShadowBiasLoc >= 0) {
                    shader->setUniform1f(lightShadowBiasLoc, light->getShadowBias());
                }

                if (lightMatrixLoc >= 0) {
                    shader->setUniformMatrix4(lightMatrixLoc, light->getOwner()->getTransform().getConstInverseWorldMatrix());
                }

                if (lightType == LightType::Point) {
                    WeakPointer<PointLight> pointLight = WeakPointer<Light>::dynamicPointerCast<PointLight>(light);
                    if (lightRangeLoc >= 0) {
                        shader->setUniform1f(lightRangeLoc, pointLight->getRadius());
                    }

                    if (lightPositionLoc >= 0) {
                        Point3r pos;
                        pointLight->getOwner()->getTransform().getWorldMatrix().transform(pos);
                        shader->setUniform4f(lightPositionLoc, pos.x, pos.y, pos.z, 1.0f);
                    }

                    if (lightShadowCubeMapLoc >= 0 && pointLight->getShadowsEnabled()) {
                        shader->setTextureCube(currentTextureSlot, pointLight->getShadowMap()->getColorTexture()->getTextureID());
                        shader->setUniform1i(lightShadowCubeMapLoc, currentTextureSlot);
                    }

                }
            }

            this->drawMesh(mesh);
        }
        else {
            if (lightEnabledLoc >= 0) {
                shader->setUniform1i(lightEnabledLoc, 0);
            }
            this->drawMesh(mesh);
        }

    }

    void MeshRenderer::render(const ViewDescriptor& viewDescriptor, const std::vector<WeakPointer<Light>>& lights) {
        std::shared_ptr<RenderableContainer<Mesh>> thisContainer = std::dynamic_pointer_cast<RenderableContainer<Mesh>>(this->owner.lock());
        if (thisContainer) {
            auto renderables = thisContainer->getRenderables();
            for (auto mesh : renderables) {
                this->renderObject(viewDescriptor, mesh, lights);
            }
        }
    }

    void MeshRenderer::checkAndSetShaderAttribute(WeakPointer<Mesh> mesh, WeakPointer<Material> material, StandardAttribute attribute, WeakPointer<AttributeArrayBase> array) {
        if (mesh->isAttributeEnabled(attribute)) {
            Int32 shaderLocation = material->getShaderLocation(attribute);
            if (array->getGPUStorage()) {
                array->getGPUStorage()->sendToShader(shaderLocation);
            }
        }
    }

    void MeshRenderer::drawMesh(WeakPointer<Mesh> mesh) {
        if (mesh->isIndexed()) {
            this->graphics->drawBoundVertexBuffer(mesh->getIndexCount(), mesh->getIndexBuffer());
        } else {
            this->graphics->drawBoundVertexBuffer(mesh->getVertexCount());
        }
    }
}