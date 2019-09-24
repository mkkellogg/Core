#include "MeshRenderer.h"
#include "../Engine.h"
#include "../geometry/AttributeArray.h"
#include "../geometry/AttributeArrayGPUStorage.h"
#include "../geometry/Mesh.h"
#include "../image/Texture.h"
#include "../image/Texture2D.h"
#include "../light/AmbientIBLLight.h"
#include "../light/PointLight.h"
#include "../light/DirectionalLight.h"
#include "../material/Material.h"
#include "../material/Shader.h"
#include "../render/Camera.h"
#include "../render/RenderTarget.h"
#include "RenderableContainer.h"
#include "RenderException.h"

namespace Core {

    MeshRenderer::MeshRenderer(WeakPointer<Graphics> graphics, WeakPointer<Material> material, WeakPointer<Object3D> owner)
        : ObjectRenderer<Mesh>(graphics, owner), material(material) {
    }

    Bool MeshRenderer::forwardRenderObject(const ViewDescriptor& viewDescriptor, WeakPointer<Mesh> mesh, const std::vector<WeakPointer<Light>>& lights,
                                           Bool matchPhysicalPropertiesWithLighting) {
        WeakPointer<Material> material;
        if (viewDescriptor.overrideMaterial.isValid()) {
            material = viewDescriptor.overrideMaterial;
        } else {
            material = this->material;
        }

        WeakPointer<Shader> shader = material->getShader();
        this->graphics->activateShader(shader);

        this->graphics->setColorWriteEnabled(material->getColorWriteEnabled());
        this->graphics->setRenderStyle(material->getRenderStyle());
        if (material->getBlendingMode() == RenderState::BlendingMode::Custom) {
            graphics->setBlendingEnabled(true);
            graphics->setBlendingFunction(material->getSourceBlendingMethod(), material->getDestBlendingMethod());
        }
        else {
            graphics->setBlendingEnabled(false);
        }
        
        graphics->setDepthWriteEnabled(material->getDepthWriteEnabled());
        graphics->setDepthTestEnabled(material->getDepthTestEnabled());
        graphics->setDepthFunction(material->getDepthFunction());

        graphics->setFaceCullingEnabled(material->getFaceCullingEnabled());
        graphics->setCullFace(material->getCullFace());

        graphics->setStencilTestEnabled(material->getStencilTestEnabled());
        graphics->setStencilWriteMask(material->getStencilWriteMask());
        if (material->getStencilTestEnabled()) {
            graphics->setStencilFunction(material->getStencilComparisonFunction(), material->getStencilRef(), material->getStencilReadMask());
            graphics->setStencilOperation(material->getStencilFailActionStencil(), material->getStencilFailActionDepth(), material->getStencilAllPassAction());
        }

        // send custom uniforms first so that the renderer can override if necessary.
        material->sendCustomUniformsToShader();

        this->checkAndSetShaderAttribute(mesh, material, StandardAttribute::Position, StandardAttribute::Position, mesh->getVertexPositions());
        this->checkAndSetShaderAttribute(mesh, material, StandardAttribute::Normal, StandardAttribute::Normal, mesh->getVertexNormals());
        this->checkAndSetShaderAttribute(mesh, material, StandardAttribute::AveragedNormal, StandardAttribute::AveragedNormal, mesh->getVertexAveragedNormals());
        this->checkAndSetShaderAttribute(mesh, material, StandardAttribute::FaceNormal, StandardAttribute::FaceNormal, mesh->getVertexFaceNormals());
        this->checkAndSetShaderAttribute(mesh, material, StandardAttribute::Tangent, StandardAttribute::Tangent, mesh->getVertexTangents());
        this->checkAndSetShaderAttribute(mesh, material, StandardAttribute::Color, StandardAttribute::Color, mesh->getVertexColors());
        this->checkAndSetShaderAttribute(mesh, material, StandardAttribute::AlbedoUV, StandardAttribute::AlbedoUV, mesh->getVertexAlbedoUVs());
        if (mesh->getVertexNormalUVs())
            this->checkAndSetShaderAttribute(mesh, material, StandardAttribute::NormalUV, StandardAttribute::NormalUV, mesh->getVertexNormalUVs());
        else
            this->checkAndSetShaderAttribute(mesh, material, StandardAttribute::AlbedoUV, StandardAttribute::NormalUV, mesh->getVertexAlbedoUVs());

        Int32 cameraPositionLoc = material->getShaderLocation(StandardUniform::CameraPosition);
        Int32 projectionLoc = material->getShaderLocation(StandardUniform::ProjectionMatrix);
        Int32 viewMatrixLoc = material->getShaderLocation(StandardUniform::ViewMatrix);
        Int32 modelMatrixLoc = material->getShaderLocation(StandardUniform::ModelMatrix);
        Int32 modelInverseTransposeMatrixLoc = material->getShaderLocation(StandardUniform::ModelInverseTransposeMatrix);
        Int32 viewInverseTransposeMatrixLoc = material->getShaderLocation(StandardUniform::ViewInverseTransposeMatrix);

        if (cameraPositionLoc >= 0) {
            shader->setUniform4f(cameraPositionLoc, viewDescriptor.cameraPosition.x, viewDescriptor.cameraPosition.y,
                                 viewDescriptor.cameraPosition.z, 1.0f);
        }

        if (projectionLoc >= 0) {
            const Matrix4x4& projMatrix = viewDescriptor.projectionMatrix;
            shader->setUniformMatrix4(projectionLoc, projMatrix);
        }

        if (viewMatrixLoc >= 0) {
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

        UInt32 currentTextureSlot = material->textureCount();

        Int32 lightEnabledLoc = material->getShaderLocation(StandardUniform::LightEnabled);
        Int32 lightShadowsEnabledLoc = material->getShaderLocation(StandardUniform::LightShadowsEnabled);

        if (lights.size() > 0 && material->isLit()) {

            if (lightEnabledLoc >= 0) {
                shader->setUniform1i(lightEnabledLoc, 1);
            }

            UInt32 renderedCount = 0;
            for (UInt32 i = 0; i < lights.size(); i++) {

                WeakPointer<Light> light = lights[i];
                LightType lightType = light->getType();
                if (lightType == LightType::AmbientIBL && !material->isPhysical()) continue;
                if (matchPhysicalPropertiesWithLighting) {
                    if (lightType == LightType::Ambient && material->isPhysical()) continue;
                }

                if (material->getBlendingMode() == RenderState::BlendingMode::Additive) {
                    if (renderedCount == 0) {
                        graphics->setBlendingEnabled(false);
                    } else {
                        graphics->setBlendingEnabled(true);
                        graphics->setBlendingFunction(RenderState::BlendingMethod::One, RenderState::BlendingMethod::One);
                    }
                }

                Int32 lightRangeLoc = material->getShaderLocation(StandardUniform::LightRange, 0);
                Int32 lightTypeLoc = material->getShaderLocation(StandardUniform::LightType, 0);
                Int32 lightIntensityLoc = material->getShaderLocation(StandardUniform::LightIntensity, 0);
                Int32 lightColorLoc = material->getShaderLocation(StandardUniform::LightColor, 0);
            
                Int32 lightMatrixLoc = material->getShaderLocation(StandardUniform::LightMatrix, 0);
                Int32 lightAngularShadowBiasLoc = material->getShaderLocation(StandardUniform::LightAngularShadowBias, 0);
                Int32 lightConstantShadowBiasLoc = material->getShaderLocation(StandardUniform::LightConstantShadowBias, 0);
                Int32 lightShadowMapSizeLoc = material->getShaderLocation(StandardUniform::LightShadowMapSize, 0);
                Int32 lightShadowSoftnessLoc = material->getShaderLocation(StandardUniform::LightShadowSoftness, 0);
                Int32 lightNearPlaneLoc = material->getShaderLocation(StandardUniform::LightNearPlane, 0);
                Int32 lightCountLoc = material->getShaderLocation(StandardUniform::LightCount, 0);

                if (lightCountLoc >= 0) {
                    shader->setUniform1i(lightCountLoc, 1);
                }

                if (lightColorLoc >= 0) {
                    Color color = light->getColor();
                    shader->setUniform4f(lightColorLoc, color.r, color.g, color.b, color.a);
                }

                if (lightTypeLoc >= 0) {
                    shader->setUniform1i(lightTypeLoc, (Int32)lightType);
                }

                if (lightIntensityLoc >= 0) {
                    shader->setUniform1f(lightIntensityLoc, light->getIntensity());
                }

                if (lightMatrixLoc >= 0) {
                    shader->setUniformMatrix4(lightMatrixLoc, light->getOwner()->getTransform().getConstInverseWorldMatrix());
                }

                Int32 irradianceMapLoc = material->getShaderLocation(StandardUniform::LightIrradianceMap);
                Int32 specularIBLPreFilteredMapLoc = material->getShaderLocation(StandardUniform::LightSpecularIBLPreFilteredMap);
                Int32 specularIBLBRDFMapLoc = material->getShaderLocation(StandardUniform::LightSpecularIBLBRDFMap);

                if (lightType == LightType::AmbientIBL) {
                    WeakPointer<AmbientIBLLight> ambientIBLLight = WeakPointer<Light>::dynamicPointerCast<AmbientIBLLight>(light);

                    if (irradianceMapLoc >= 0) {
                        shader->setTextureCube(currentTextureSlot, irradianceMapLoc, ambientIBLLight->getIrradianceMap()->getTextureID());
                        currentTextureSlot++;
                    }
                    
                    if (specularIBLPreFilteredMapLoc >= 0) {
                        shader->setTextureCube(currentTextureSlot, specularIBLPreFilteredMapLoc, ambientIBLLight->getSpecularIBLPreFilteredMap()->getTextureID());
                        currentTextureSlot++;
                    }
                    
                    if (specularIBLBRDFMapLoc >= 0) {
                        shader->setTexture2D(currentTextureSlot, specularIBLBRDFMapLoc, ambientIBLLight->getSpecularIBLBRDFMap()->getTextureID());
                        currentTextureSlot++;
                    }
                } else {
                    if (irradianceMapLoc >= 0) {
                        shader->setTextureCube(currentTextureSlot, irradianceMapLoc, this->graphics->getPlaceHolderCubeTexture()->getTextureID());
                        currentTextureSlot++;
                    }
                    
                    if (specularIBLPreFilteredMapLoc >= 0) {
                        shader->setTextureCube(currentTextureSlot, specularIBLPreFilteredMapLoc, this->graphics->getPlaceHolderCubeTexture()->getTextureID());
                        currentTextureSlot++;
                    }
                    
                    if (specularIBLBRDFMapLoc >= 0) {
                        shader->setTexture2D(currentTextureSlot, specularIBLBRDFMapLoc, this->graphics->getPlaceHolderTexture2D()->getTextureID());
                        currentTextureSlot++;
                    }
                }

                if (lightType == LightType::Point || lightType == LightType::Directional) {
                    WeakPointer<ShadowLight> shadowLight = WeakPointer<Light>::dynamicPointerCast<ShadowLight>(light);

                    if (lightAngularShadowBiasLoc >= 0) {
                        shader->setUniform1f(lightAngularShadowBiasLoc, shadowLight->getAngularShadowBias());
                    }

                    if (lightShadowMapSizeLoc >= 0) {
                        shader->setUniform1f(lightShadowMapSizeLoc, shadowLight->getShadowMapSize());
                    }

                    if (lightConstantShadowBiasLoc >= 0) {
                        shader->setUniform1f(lightConstantShadowBiasLoc, shadowLight->getConstantShadowBias());
                    }

                    if (lightShadowSoftnessLoc >= 0) {
                        shader->setUniform1i(lightShadowSoftnessLoc, (UInt32)shadowLight->getShadowSoftness());
                    }
                }

                Int32 lightShadowCubeMapLoc = material->getShaderLocation(StandardUniform::LightShadowCubeMap);
                if (lightType == LightType::Point) {

                    WeakPointer<PointLight> pointLight = WeakPointer<Light>::dynamicPointerCast<PointLight>(light);

                    if (lightRangeLoc >= 0) {
                        shader->setUniform1f(lightRangeLoc, pointLight->getRadius());
                    }

                    if (lightNearPlaneLoc >= 0) {
                        shader->setUniform1f(lightNearPlaneLoc, PointLight::NearPlane);
                    }

                    Int32 lightPositionLoc = material->getShaderLocation(StandardUniform::LightPosition);
                    if (lightPositionLoc >= 0) {
                        Point3r pos;
                        pointLight->getOwner()->getTransform().applyTransformationTo(pos);
                        shader->setUniform4f(lightPositionLoc, pos.x, pos.y, pos.z, 1.0f);
                    }

                    if (lightShadowsEnabledLoc >= 0) {
                        shader->setUniform1i(lightShadowsEnabledLoc, pointLight->getShadowsEnabled() ? 1.0 : 0.0);
                    } 

                    if (lightShadowCubeMapLoc >= 0) {
                        if (pointLight->getShadowsEnabled())
                            shader->setTextureCube(currentTextureSlot, lightShadowCubeMapLoc, pointLight->getShadowMap()->getColorTexture()->getTextureID());
                        else
                            shader->setTextureCube(currentTextureSlot, lightShadowCubeMapLoc, this->graphics->getPlaceHolderCubeTexture()->getTextureID());
                        currentTextureSlot++;
                    }
                } else {
                    if (lightShadowCubeMapLoc >= 0) {
                        shader->setTextureCube(currentTextureSlot, lightShadowCubeMapLoc, this->graphics->getPlaceHolderCubeTexture()->getTextureID());
                        currentTextureSlot++;
                    }
                }
                
                
                if (lightType == LightType::Directional) {
                    WeakPointer<DirectionalLight> directionalLight = WeakPointer<Light>::dynamicPointerCast<DirectionalLight>(light);

                    Int32 lightDirectionLoc = material->getShaderLocation(StandardUniform::LightDirection);
                    if (lightDirectionLoc >= 0) {
                        Vector3r dir = Vector3r::Forward;
                        directionalLight->getOwner()->getTransform().applyTransformationTo(dir);
                        shader->setUniform4f(lightDirectionLoc, dir.x, dir.y, dir.z, 0.0f);
                    }

                    UInt32 cascadeCount = directionalLight->getCascadeCount();

                    Int32 cascadeCountLoc = material->getShaderLocation(StandardUniform::LightCascadeCount);
                    if (cascadeCountLoc >= 0) {
                        shader->setUniform1i(cascadeCountLoc, cascadeCount);
                    }

                    if (lightShadowsEnabledLoc >= 0) {
                        shader->setUniform1i(lightShadowsEnabledLoc, directionalLight->getShadowsEnabled() ? 1.0 : 0.0);
                    } 

                    for (UInt32 l = 0; l < cascadeCount; l++) {
                        Int32 shadowMapLoc = material->getShaderLocation(StandardUniform::LightShadowMap, l);
                        if (shadowMapLoc >= 0) {
                            if (directionalLight->getShadowsEnabled()) {
                                shader->setTexture2D(currentTextureSlot, shadowMapLoc, directionalLight->getShadowMap(l)->getDepthTexture()->getTextureID());
                            } else {
                                shader->setTexture2D(currentTextureSlot, shadowMapLoc, this->graphics->getPlaceHolderTexture2D()->getTextureID());
                            }
                            currentTextureSlot++;
                        } 

                        Int32 viewProjectionLoc = material->getShaderLocation(StandardUniform::LightViewProjection, l);
                        if (viewProjectionLoc >= 0) {
                            shader->setUniformMatrix4(viewProjectionLoc, directionalLight->getViewProjectionMatrix(l));
                        }

                        Int32 cascadeEndLoc = material->getShaderLocation(StandardUniform::LightCascadeEnd, l);
                        if (cascadeEndLoc >= 0) {
                            shader->setUniform1f(cascadeEndLoc, directionalLight->getCascadeBoundary(l + 1));
                        }

                        Int32 lightShadowMapAspectLoc = material->getShaderLocation(StandardUniform::LightShadowMapAspect, l);
                        if (lightShadowMapAspectLoc >= 0) {
                            DirectionalLight::OrthoProjection& proj = directionalLight->getProjection(l);
                            Real aspect = Math::abs((proj.right - proj.left) / (proj.top - proj.bottom));
                            shader->setUniform1f(lightShadowMapAspectLoc, aspect);
                        }
                    }

                    for (UInt32 l = cascadeCount; l < Constants::MaxDirectionalCascades; l++) {
                        Int32 shadowMapLoc = material->getShaderLocation(StandardUniform::LightShadowMap, l);
                        if (shadowMapLoc >= 0) {
                            shader->setTexture2D(currentTextureSlot, shadowMapLoc, this->graphics->getPlaceHolderTexture2D()->getTextureID());
                            currentTextureSlot++;
                        }
                    }
                } else {
                    for (UInt32 l = 0; l < Constants::MaxDirectionalCascades; l++) {
                        Int32 shadowMapLoc = material->getShaderLocation(StandardUniform::LightShadowMap, l);
                        if (shadowMapLoc >= 0) {
                            shader->setTexture2D(currentTextureSlot, shadowMapLoc, this->graphics->getPlaceHolderTexture2D()->getTextureID());
                            currentTextureSlot++;
                        }
                    }
                }
                renderedCount++;
                this->drawMesh(mesh);
            }

        } else {
            if (material->isLit()) {
                throw RenderException("MeshRenderer::render() -> Rendering lit material with no lights!");    
            }

            if (lightEnabledLoc >= 0) {
                shader->setUniform1i(lightEnabledLoc, 0);
            }
            this->drawMesh(mesh);
        }

        this->disableShaderAttribute(mesh, material, StandardAttribute::Position, mesh->getVertexPositions());
        this->disableShaderAttribute(mesh, material, StandardAttribute::Normal, mesh->getVertexNormals());
        this->disableShaderAttribute(mesh, material, StandardAttribute::AveragedNormal, mesh->getVertexAveragedNormals());
        this->disableShaderAttribute(mesh, material, StandardAttribute::FaceNormal, mesh->getVertexFaceNormals());
        this->disableShaderAttribute(mesh, material, StandardAttribute::Tangent, mesh->getVertexTangents());
        this->disableShaderAttribute(mesh, material, StandardAttribute::Color, mesh->getVertexColors());
        this->disableShaderAttribute(mesh, material, StandardAttribute::AlbedoUV, mesh->getVertexAlbedoUVs());
        this->disableShaderAttribute(mesh, material, StandardAttribute::NormalUV, mesh->getVertexNormalUVs());

        return true;
    }

    Bool MeshRenderer::forwardRender(const ViewDescriptor& viewDescriptor, const std::vector<WeakPointer<Light>>& lights,
                                     Bool matchPhysicalPropertiesWithLighting) {
        std::shared_ptr<RenderableContainer<Mesh>> thisContainer = std::dynamic_pointer_cast<RenderableContainer<Mesh>>(this->owner.lock());
        if (thisContainer) {
            auto renderables = thisContainer->getRenderables();
            for (auto mesh : renderables) {
                this->forwardRenderObject(viewDescriptor, mesh, lights, matchPhysicalPropertiesWithLighting);
            }
        }

        return true;
    }

    Bool MeshRenderer::supportsRenderPath(RenderPath renderPath) {
        if (renderPath == RenderPath::Forward) return true;
        return false;
    }

    void MeshRenderer::setMaterial(WeakPointer<Material> material) {
        this->material = material;
    }

    WeakPointer<Material> MeshRenderer::getMaterial() {
        return this->material;
    }

    void MeshRenderer::checkAndSetShaderAttribute(WeakPointer<Mesh> mesh, WeakPointer<Material> material, StandardAttribute checkAttribute,
                                                  StandardAttribute setAttribute, WeakPointer<AttributeArrayBase> array) {
        if (mesh->isAttributeEnabled(checkAttribute)) {
            Int32 shaderLocation = material->getShaderLocation(setAttribute);
            if (array->getGPUStorage()) {
                array->getGPUStorage()->sendToShader(shaderLocation);
            }
        }
    }

     void MeshRenderer::disableShaderAttribute(WeakPointer<Mesh> mesh, WeakPointer<Material> material, StandardAttribute attribute, 
                                              WeakPointer<AttributeArrayBase> array) {
        if (mesh->isAttributeEnabled(attribute)) {
            Int32 shaderLocation = material->getShaderLocation(attribute);
            if (array->getGPUStorage()) {
                array->getGPUStorage()->disable(shaderLocation);
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