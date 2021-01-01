#include "MeshRenderer.h"
#include "BaseRenderable.h"
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
#include "../material/MaterialState.h"
#include "../material/Shader.h"
#include "../render/Camera.h"
#include "../render/RenderTarget.h"
#include "MeshContainer.h"
#include "../animation/VertexBoneMap.h"
#include "../animation/Bone.h"
#include "../animation/Object3DSkeletonNode.h"
#include "RenderException.h"
#include "RenderPath.h"
#include "DepthOutputOverride.h"
#include "RenderUtils.h"

namespace Core {

    MeshRenderer::MeshRenderer(WeakPointer<Material> material, WeakPointer<Object3D> owner)
        : Object3DRenderer<Mesh>(owner), material(material) {
    }

    MeshRenderer::~MeshRenderer() {
        if (this->material.isValid()) {
            Engine::safeReleaseObject(this->material);
        }
    }

    Bool MeshRenderer::init() {
        return true;
    }

    void MeshRenderer::preProcess() {
        WeakPointer<MeshContainer> meshContainer = this->owner->getMeshContainer();
        if (meshContainer.isValid()) {
            Matrix4x4 rootTransformInverse = this->owner->getTransform().getWorldMatrix();
            rootTransformInverse.invert();

            Matrix4x4 temp;
            WeakPointer<Skeleton> skeleton = meshContainer->getSkeleton();
            if (skeleton.isValid()) {
                for(UInt32 i = 0; i < skeleton->getNodeCount(); i++) {
                    Skeleton::SkeletonNode * node = skeleton->getNodeFromList(i);
                    if (node->BoneIndex >= 0) {
                        Int32 bonesLocation = material->getShaderLocation(StandardUniform::Bones, node->BoneIndex);
                        if (bonesLocation >= 0) {
                            temp.copy(skeleton->getBone(node->BoneIndex)->OffsetMatrix);
                            temp.preMultiply(node->getFullTransform());
                            temp.preMultiply(rootTransformInverse);
                            node->TempRenderTransformation.copy(temp);
                        }
                    }
                }
            }
        }
    }

    void MeshRenderer::setRenderStateForMaterial(WeakPointer<Material> material, Bool renderingDepthOutput) {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        graphics->setColorWriteEnabled(material->getColorWriteEnabled());
        graphics->setRenderStyle(material->getRenderStyle());
        if (material->getBlendingMode() != RenderState::BlendingMode::None && !renderingDepthOutput) {
            graphics->setBlendingEnabled(true);
            if (material->getBlendingMode() == RenderState::BlendingMode::Custom) {
                graphics->setBlendingEquation(material->getBlendingEquation());
            }
            graphics->setBlendingFactors(material->getSourceBlendingFactor(), material->getDestBlendingFactor());
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
    }

    void MeshRenderer::setSkinningVars(WeakPointer<Mesh> mesh, WeakPointer<Material> material, WeakPointer<Shader> shader) {
        Int32 skinningEnabledLocation = material->getShaderLocation(StandardUniform::SkinningEnabled);
        if (skinningEnabledLocation >= 0) shader->setUniform1i(skinningEnabledLocation, 0.0);
        if (material->isSkinningEnabled()) {
            WeakPointer<MeshContainer> meshContainer = this->owner->getMeshContainer();
            if (meshContainer.isValid() && meshContainer->hasVertexBoneMap(mesh->getObjectID())) {
                if (skinningEnabledLocation >= 0) shader->setUniform1i(skinningEnabledLocation, 1.0);
                WeakPointer<VertexBoneMap> vertexBoneMap = meshContainer->getVertexBoneMap(mesh->getObjectID());
                this->checkAndSetShaderAttribute(mesh, material, StandardAttribute::BoneIndex, StandardAttribute::BoneIndex, vertexBoneMap->getIndices(), true);
                this->checkAndSetShaderAttribute(mesh, material, StandardAttribute::BoneWeight, StandardAttribute::BoneWeight, vertexBoneMap->getWeights(), true);

                WeakPointer<Skeleton> skeleton = meshContainer->getSkeleton();
                for(UInt32 i = 0; i < skeleton->getNodeCount(); i++) {
                    Skeleton::SkeletonNode * node = skeleton->getNodeFromList(i);
                    if (node->BoneIndex >= 0) {
                        Int32 bonesLocation = material->getShaderLocation(StandardUniform::Bones, node->BoneIndex);
                        if (bonesLocation >= 0) {
                            shader->setUniformMatrix4(bonesLocation, node->TempRenderTransformation);
                        }
                    }
                }
            }
        }
    }

     Bool MeshRenderer::forwardRenderObject(const ViewDescriptor& viewDescriptor, WeakPointer<BaseRenderable> renderable, Bool isStatic,
                                            Int32 layer, const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        WeakPointer<Mesh> mesh = WeakPointer<BaseRenderable>::dynamicPointerCast<Mesh>(renderable);
        if (!mesh.isValid()) {
            throw RenderException("MeshRenderer::forwardRenderObject() -> 'renderable' is not an instance of Mesh!");  
        }

        this->forwardRenderMesh(viewDescriptor, mesh, isStatic, layer, lightPack, matchPhysicalPropertiesWithLighting);
    }

    Bool MeshRenderer::forwardRenderMesh(const ViewDescriptor& viewDescriptor, WeakPointer<Mesh> mesh, Bool isStatic,
                                         Int32 layer, const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {

        Matrix4x4 tempMatrix;
        WeakPointer<Material> material;
        Bool copiedStateFromOverrideMaterial = false;
        Bool usingOverrideMaterial = false;
        Bool savedOverrideMaterialSkinningEnabled = false;
        MaterialState savedState;
        Bool renderingDepthOutput = this->material->hasCustomDepthOutput() && viewDescriptor.depthOutputOverride != DepthOutputOverride::None;
        if (!renderingDepthOutput && viewDescriptor.overrideMaterial.isValid()) {
            material = viewDescriptor.overrideMaterial;
            savedOverrideMaterialSkinningEnabled = material->isSkinningEnabled();
            material->setSkinningEnabled(this->material->isSkinningEnabled());
            usingOverrideMaterial = true;
        } else {
            material = this->material;
            if (material->getCustomDepthOutputCopyOverrideMatrialState() && viewDescriptor.overrideMaterial.isValid()) {
                savedState = material->getState();
                material->setState(viewDescriptor.overrideMaterial->getState());
                if (material->getCustomDepthOutputStateCopyExcludeFaceCulling()) {
                    material->setFaceCullingEnabled(savedState.faceCullingEnabled);
                    material->setCullFace(savedState.cullFace);
                }
                copiedStateFromOverrideMaterial = true;
            }
        }
        WeakPointer<Shader> shader = material->getShader();
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        graphics->activateShader(shader);

        this->setRenderStateForMaterial(material, renderingDepthOutput);
        
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

        this->setSkinningVars(mesh, material, shader);

        Int32 cameraPositionLoc = material->getShaderLocation(StandardUniform::CameraPosition);
        if (cameraPositionLoc >= 0) {
            shader->setUniform4f(cameraPositionLoc, viewDescriptor.cameraPosition.x, viewDescriptor.cameraPosition.y,
                                 viewDescriptor.cameraPosition.z, 1.0f);
        }

        Int32 projectionLoc = material->getShaderLocation(StandardUniform::ProjectionMatrix);
        Int32 viewMatrixLoc = material->getShaderLocation(StandardUniform::ViewMatrix);
        Int32 modelMatrixLoc = material->getShaderLocation(StandardUniform::ModelMatrix);
        Int32 modelInverseTransposeMatrixLoc = material->getShaderLocation(StandardUniform::ModelInverseTransposeMatrix);
        Int32 viewInverseTransposeMatrixLoc = material->getShaderLocation(StandardUniform::ViewInverseTransposeMatrix);
        if (projectionLoc >= 0) shader->setUniformMatrix4(projectionLoc, viewDescriptor.projectionMatrix);
        if (viewMatrixLoc >= 0) shader->setUniformMatrix4(viewMatrixLoc, viewDescriptor.inverseCameraTransformation);
        if (modelMatrixLoc >= 0) shader->setUniformMatrix4(modelMatrixLoc, this->owner->getTransform().getWorldMatrix());
        if (modelInverseTransposeMatrixLoc >= 0) {
            Matrix4x4 modelInverseTransposeMatrix = this->owner->getTransform().getWorldMatrix();
            modelInverseTransposeMatrix.invert();
            modelInverseTransposeMatrix.transpose();
            shader->setUniformMatrix4(modelInverseTransposeMatrixLoc, modelInverseTransposeMatrix);
        }
        if (viewInverseTransposeMatrixLoc >= 0) shader->setUniformMatrix4(viewInverseTransposeMatrixLoc, viewDescriptor.transposedCameraTransformation);


        UInt32 baseTextureSlot = material->textureCount();
        Int32 ssaoMapLoc = material->getShaderLocation(StandardUniform::SSAOMap);
        Int32 ssaoEnabledLoc = material->getShaderLocation(StandardUniform::SSAOEnabled);
        if (ssaoMapLoc >= 0) {
            if (isStatic && viewDescriptor.ssaoEnabled && viewDescriptor.ssaoMap.isValid()) {
                shader->setTexture2D(baseTextureSlot, ssaoMapLoc, viewDescriptor.ssaoMap->getTextureID());
                if (ssaoEnabledLoc >= 0) shader->setUniform1i(ssaoEnabledLoc, 1.0);
            } else {
                shader->setTexture2D(baseTextureSlot, ssaoMapLoc, graphics->getPlaceHolderTexture2D()->getTextureID());
                if (ssaoEnabledLoc >= 0) shader->setUniform1i(ssaoEnabledLoc, 0.0);
            }
            baseTextureSlot++;
        } else {
            if (ssaoEnabledLoc >= 0) shader->setUniform1i(ssaoEnabledLoc, 0.0);
        }

        Int32 depthOutputOverrideLoc = material->getShaderLocation(StandardUniform::DepthOutputOverride); 
        if (depthOutputOverrideLoc >= 0) {
            shader->setUniform1i(depthOutputOverrideLoc, (UInt32)viewDescriptor.depthOutputOverride);
        }

        RenderPath renderPath = material->getRenderPath();
        if (lightPack.lightCount() > 0 && material->isLit() && !renderingDepthOutput) {

            Int32 lightCountLoc = material->getShaderLocation(StandardUniform::LightCount);
            UInt32 renderPassCount = 0;

            Int32 directionalLightIndex = -1;
            Int32 pointLightIndex = -1;
            Int32 ambientLightIndex = -1;
            Int32 ambientIBLLightIndex = -1;
            Int32 shadowLightIndex = -1;

            UInt32 currentTextureSlot = baseTextureSlot;
            for (UInt32 i = 0; i < lightPack.lightCount(); i++) {

                WeakPointer<Light> light = lightPack.getLight(i);
                LightType lightType = light->getType();

                if (renderPath != RenderPath::SinglePassMultiLight) currentTextureSlot = baseTextureSlot;

                switch (lightType) {
                    case LightType::Directional:
                        directionalLightIndex++;
                        shadowLightIndex++;
                    break;
                    case LightType::Point:
                        pointLightIndex++;
                        shadowLightIndex++;
                    break;
                    case LightType::Ambient:
                        ambientLightIndex++;
                    break;
                    case LightType::AmbientIBL:
                        ambientIBLLightIndex++;
                    break;
                }

                IntMask cullingMask = light->getCullingMask();
                Bool layerValidForLight = IntMaskUtil::isBitSet(cullingMask, layer);
                if (!layerValidForLight) continue;

                if (lightType == LightType::AmbientIBL && !material->isPhysical()) continue;
                if (matchPhysicalPropertiesWithLighting) {
                    if (lightType == LightType::Ambient && material->isPhysical()) continue;
                }
                if (renderPath == RenderPath::SinglePassMultiLight && (renderPassCount >= material->maxLightCount() || renderPassCount >= Constants::MaxShaderLights)) continue;
                
                Point3r pointLightPos;
                if (lightType == LightType::Point) {
                    WeakPointer<PointLight> pointLight = lightPack.getPointLight(pointLightIndex);
                    pointLightPos.set(0.0f, 0.0f, 0.0f);
                    pointLight->getOwner()->getTransform().applyTransformationTo(pointLightPos);
                    if (!RenderUtils::isPointLightInRangeOfMesh(pointLightPos, pointLight->getRadius(), mesh, this->owner)) continue;
                }

                if (renderPath != RenderPath::SinglePassMultiLight) {
                    if (material->getBlendingMode() == RenderState::BlendingMode::None) {
                        if (renderPassCount == 0) {
                            graphics->setBlendingEnabled(false);
                        } else {
                            graphics->setBlendingEnabled(true);
                            graphics->setBlendingFactors(RenderState::BlendingFactor::One, RenderState::BlendingFactor::One);
                        }
                    }
                }

                Int32 lightShaderVarLocOffset = renderPath != RenderPath::SinglePassMultiLight ? 0 : renderPassCount;

                Int32 lightEnabledLoc = material->getShaderLocation(StandardUniform::LightEnabled, lightShaderVarLocOffset);
                if (lightEnabledLoc >= 0) shader->setUniform1i(lightEnabledLoc, 1);

                Int32 lightShadowsEnabledLoc = material->getShaderLocation(StandardUniform::LightShadowsEnabled, lightShaderVarLocOffset);
                Int32 lightRangeLoc = material->getShaderLocation(StandardUniform::LightRange, lightShaderVarLocOffset);
                Int32 lightTypeLoc = material->getShaderLocation(StandardUniform::LightType, lightShaderVarLocOffset);
                Int32 lightIntensityLoc = material->getShaderLocation(StandardUniform::LightIntensity, lightShaderVarLocOffset);
                Int32 lightColorLoc = material->getShaderLocation(StandardUniform::LightColor, lightShaderVarLocOffset);
                Int32 lightMatrixLoc = material->getShaderLocation(StandardUniform::LightMatrix, lightShaderVarLocOffset);
                Int32 lightAngularShadowBiasLoc = material->getShaderLocation(StandardUniform::LightAngularShadowBias, lightShaderVarLocOffset);
                Int32 lightConstantShadowBiasLoc = material->getShaderLocation(StandardUniform::LightConstantShadowBias, lightShaderVarLocOffset);
                Int32 lightShadowMapSizeLoc = material->getShaderLocation(StandardUniform::LightShadowMapSize, lightShaderVarLocOffset);
                Int32 lightShadowSoftnessLoc = material->getShaderLocation(StandardUniform::LightShadowSoftness, lightShaderVarLocOffset);
                Int32 lightNearPlaneLoc = material->getShaderLocation(StandardUniform::LightNearPlane, lightShaderVarLocOffset);

                if (lightColorLoc >= 0) {
                    Color color = light->getColor();
                    shader->setUniform4f(lightColorLoc, color.r, color.g, color.b, color.a);
                }

                if (lightTypeLoc >= 0) shader->setUniform1i(lightTypeLoc, (Int32)lightType);
                if (lightIntensityLoc >= 0) shader->setUniform1f(lightIntensityLoc, light->getIntensity());

                if (lightMatrixLoc >= 0) {
                    tempMatrix.copy(light->getOwner()->getTransform().getWorldMatrix());
                    tempMatrix.invert();
                    shader->setUniformMatrix4(lightMatrixLoc, tempMatrix);
                }

                Int32 irradianceMapLoc = material->getShaderLocation(StandardUniform::LightIrradianceMap, lightShaderVarLocOffset);
                Int32 specularIBLPreFilteredMapLoc = material->getShaderLocation(StandardUniform::LightSpecularIBLPreFilteredMap, lightShaderVarLocOffset);
                Int32 specularIBLBRDFMapLoc = material->getShaderLocation(StandardUniform::LightSpecularIBLBRDFMap, lightShaderVarLocOffset);

                if (lightType == LightType::AmbientIBL) {
                    WeakPointer<AmbientIBLLight> ambientIBLLight = lightPack.getAmbientIBLLight(ambientIBLLightIndex);
                    this->testAndSetTextureCubeWithInc(shader, currentTextureSlot, irradianceMapLoc, ambientIBLLight->getIrradianceMap()->getTextureID());
                    this->testAndSetTextureCubeWithInc(shader, currentTextureSlot, specularIBLPreFilteredMapLoc, ambientIBLLight->getSpecularIBLPreFilteredMap()->getTextureID());
                    this->testAndSetTexture2DWithInc(shader, currentTextureSlot, specularIBLBRDFMapLoc, ambientIBLLight->getSpecularIBLBRDFMap()->getTextureID());
                } else {
                    this->testAndSetTextureCubeWithInc(shader, currentTextureSlot, irradianceMapLoc, graphics->getPlaceHolderCubeTexture()->getTextureID());
                    this->testAndSetTextureCubeWithInc(shader, currentTextureSlot, specularIBLPreFilteredMapLoc, graphics->getPlaceHolderCubeTexture()->getTextureID());
                    this->testAndSetTexture2DWithInc(shader, currentTextureSlot, specularIBLBRDFMapLoc, graphics->getPlaceHolderTexture2D()->getTextureID());
                }

                if (lightType == LightType::Point || lightType == LightType::Directional) {
                    WeakPointer<ShadowLight> shadowLight = lightPack.getShadowLight(shadowLightIndex);
                    if (lightAngularShadowBiasLoc >= 0) shader->setUniform1f(lightAngularShadowBiasLoc, shadowLight->getAngularShadowBias());
                    if (lightShadowMapSizeLoc >= 0) shader->setUniform1f(lightShadowMapSizeLoc, shadowLight->getShadowMapSize());
                    if (lightConstantShadowBiasLoc >= 0) shader->setUniform1f(lightConstantShadowBiasLoc, shadowLight->getConstantShadowBias());
                    if (lightShadowSoftnessLoc >= 0) shader->setUniform1i(lightShadowSoftnessLoc, (UInt32)shadowLight->getShadowSoftness());
                }

                Int32 lightShadowCubeMapLoc = material->getShaderLocation(StandardUniform::LightShadowCubeMap, lightShaderVarLocOffset);
                Int32 shadowMapTextureID = graphics->getPlaceHolderCubeTexture()->getTextureID();
                if (lightType == LightType::Point) {
                    WeakPointer<PointLight> pointLight = lightPack.getPointLight(pointLightIndex);
                    if (lightRangeLoc >= 0) shader->setUniform1f(lightRangeLoc, pointLight->getRadius());
                    if (lightNearPlaneLoc >= 0) shader->setUniform1f(lightNearPlaneLoc, PointLight::NearPlane);
                    Int32 lightPositionLoc = material->getShaderLocation(StandardUniform::LightPosition, lightShaderVarLocOffset);
                    if (lightPositionLoc >= 0) shader->setUniform4f(lightPositionLoc, pointLightPos.x, pointLightPos.y, pointLightPos.z, 1.0f);
                    if (lightShadowsEnabledLoc >= 0) shader->setUniform1i(lightShadowsEnabledLoc, pointLight->getShadowsEnabled() ? 1.0 : 0.0);
                    shadowMapTextureID = pointLight->getShadowsEnabled() ? pointLight->getShadowMap()->getColorTexture()->getTextureID() :
                                                                           graphics->getPlaceHolderCubeTexture()->getTextureID();
                }
                this->testAndSetTextureCubeWithInc(shader, currentTextureSlot, lightShadowCubeMapLoc, shadowMapTextureID);

                UInt32 cascadeCount = 0;
                if (lightType == LightType::Directional) {
                    WeakPointer<DirectionalLight> directionalLight = lightPack.getDirectionalLight(directionalLightIndex);

                    Int32 lightDirectionLoc = material->getShaderLocation(StandardUniform::LightDirection, lightShaderVarLocOffset);
                    if (lightDirectionLoc >= 0) {
                        Vector3r dir = Vector3r::Forward;
                        directionalLight->getOwner()->getTransform().applyTransformationTo(dir);
                        shader->setUniform4f(lightDirectionLoc, dir.x, dir.y, dir.z, 0.0f);
                    }

                    cascadeCount = directionalLight->getCascadeCount();
                    Int32 cascadeCountLoc = material->getShaderLocation(StandardUniform::LightCascadeCount, lightShaderVarLocOffset);
                    if (cascadeCountLoc >= 0) shader->setUniform1i(cascadeCountLoc, cascadeCount);
                    if (lightShadowsEnabledLoc >= 0) shader->setUniform1i(lightShadowsEnabledLoc, directionalLight->getShadowsEnabled() ? 1.0 : 0.0);

                    for (UInt32 l = 0; l < cascadeCount; l++) {
                        Int32 lightCascadeShaderVarLocOffset = lightShaderVarLocOffset * Constants::MaxDirectionalCascades + l;
                        Int32 shadowMapLoc = material->getShaderLocation(StandardUniform::LightShadowMap, lightCascadeShaderVarLocOffset);
                        shadowMapTextureID = directionalLight->getShadowsEnabled() ? directionalLight->getShadowMap(l)->getDepthTexture()->getTextureID() :
                                                                                     graphics->getPlaceHolderTexture2D()->getTextureID();
                        this->testAndSetTexture2DWithInc(shader, currentTextureSlot, shadowMapLoc, shadowMapTextureID);
                        Int32 viewProjectionLoc = material->getShaderLocation(StandardUniform::LightViewProjection, lightCascadeShaderVarLocOffset);
                        if (viewProjectionLoc >= 0) shader->setUniformMatrix4(viewProjectionLoc, directionalLight->getViewProjectionMatrix(l));
                        Int32 cascadeEndLoc = material->getShaderLocation(StandardUniform::LightCascadeEnd, lightCascadeShaderVarLocOffset);
                        if (cascadeEndLoc >= 0) shader->setUniform1f(cascadeEndLoc, directionalLight->getCascadeBoundary(l + 1));

                        Int32 lightShadowMapAspectLoc = material->getShaderLocation(StandardUniform::LightShadowMapAspect, lightCascadeShaderVarLocOffset);
                        if (lightShadowMapAspectLoc >= 0) {
                            DirectionalLight::OrthoProjection& proj = directionalLight->getProjection(l);
                            Real aspect = Math::abs((proj.right - proj.left) / (proj.top - proj.bottom));
                            shader->setUniform1f(lightShadowMapAspectLoc, aspect);
                        }
                    }
                }

                for (UInt32 l = cascadeCount; l < Constants::MaxDirectionalCascades; l++) {
                    Int32 lightCascadeShaderVarLocOffset = lightShaderVarLocOffset * Constants::MaxDirectionalCascades + l;
                    Int32 shadowMapLoc = material->getShaderLocation(StandardUniform::LightShadowMap, lightCascadeShaderVarLocOffset);
                    this->testAndSetTexture2DWithInc(shader, currentTextureSlot, shadowMapLoc, graphics->getPlaceHolderTexture2D()->getTextureID());
                }

                if (renderPath != RenderPath::SinglePassMultiLight) {
                    if (lightCountLoc >= 0) shader->setUniform1i(lightCountLoc, 1);
                    this->drawMesh(mesh);
                }
                renderPassCount++;
            }
    
            if (renderPath == RenderPath::SinglePassMultiLight) {
                if (lightCountLoc >= 0) shader->setUniform1i(lightCountLoc, renderPassCount);
                this->drawMesh(mesh);
            }

        } else {
            if (material->isLit() && !renderingDepthOutput) {
                throw RenderException("MeshRenderer::render() -> Rendering lit material with no lights!");    
            }

            Int32 lightCountLoc = material->getShaderLocation(StandardUniform::LightCount);
            Int32 lightEnabledLoc = material->getShaderLocation(StandardUniform::LightEnabled, 0);
            Int32 lightShadowsEnabledLoc = material->getShaderLocation(StandardUniform::LightShadowsEnabled, 0);

            if (lightEnabledLoc >= 0) shader->setUniform1i(lightEnabledLoc, 0);
            if (lightShadowsEnabledLoc >= 0) shader->setUniform1i(lightShadowsEnabledLoc, 0);
            if (lightCountLoc >= 0) shader->setUniform1i(lightCountLoc, 0);

            this->drawMesh(mesh);
        }

        if (copiedStateFromOverrideMaterial) {
            material->setState(savedState);
        }

        this->disableShaderAttribute(mesh, material, StandardAttribute::Position, mesh->getVertexPositions());
        this->disableShaderAttribute(mesh, material, StandardAttribute::Normal, mesh->getVertexNormals());
        this->disableShaderAttribute(mesh, material, StandardAttribute::AveragedNormal, mesh->getVertexAveragedNormals());
        this->disableShaderAttribute(mesh, material, StandardAttribute::FaceNormal, mesh->getVertexFaceNormals());
        this->disableShaderAttribute(mesh, material, StandardAttribute::Tangent, mesh->getVertexTangents());
        this->disableShaderAttribute(mesh, material, StandardAttribute::Color, mesh->getVertexColors());
        this->disableShaderAttribute(mesh, material, StandardAttribute::AlbedoUV, mesh->getVertexAlbedoUVs());
        this->disableShaderAttribute(mesh, material, StandardAttribute::NormalUV, mesh->getVertexNormalUVs());

         if (material->isSkinningEnabled()) {
            WeakPointer<MeshContainer> meshContainer = this->owner->getMeshContainer();
            if (meshContainer) {
                if (meshContainer->hasVertexBoneMap(mesh->getObjectID())) {
                    WeakPointer<VertexBoneMap> vertexBoneMap = meshContainer->getVertexBoneMap(mesh->getObjectID());
                    this->disableShaderAttribute(mesh, material, StandardAttribute::BoneIndex, vertexBoneMap->getIndices());
                    this->disableShaderAttribute(mesh, material, StandardAttribute::BoneWeight, vertexBoneMap->getWeights());
                }
            }
        }

        if (usingOverrideMaterial) {
            material->setSkinningEnabled(savedOverrideMaterialSkinningEnabled);
        }

        return true;
    }

    Bool MeshRenderer::forwardRender(const ViewDescriptor& viewDescriptor, const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        WeakPointer<MeshContainer> meshContainer = this->owner->getMeshContainer();
        Int32 layer = this->owner->getLayer();
        Bool isStatic = this->owner->isStatic();
        if (meshContainer) {
            UInt32 renderableCount = meshContainer->getBaseRenderableCount();
            for (UInt32 i = 0; i < renderableCount; i++) {
                WeakPointer<BaseRenderable> mesh = meshContainer->getBaseRenderable(i);
                this->forwardRenderObject(viewDescriptor, mesh, isStatic, layer, lightPack, matchPhysicalPropertiesWithLighting);
            }
        }

        return true;
    }

    Bool MeshRenderer::supportsRenderPath(RenderPath renderPath) {
        if (renderPath == RenderPath::Forward) return true;
        return false;
    }

    UInt32 MeshRenderer::getRenderQueueID() const {
        this->material->getRenderQueueID();
    }

    void MeshRenderer::setMaterial(WeakPointer<Material> material) {
        this->material = material;
    }

    WeakPointer<Material> MeshRenderer::getMaterial() {
        return this->material;
    }

    void MeshRenderer::checkAndSetShaderAttribute(WeakPointer<Mesh> mesh, WeakPointer<Material> material, StandardAttribute checkAttribute,
                                                  StandardAttribute setAttribute, WeakPointer<AttributeArrayBase> array, Bool force) {
        if (mesh->isAttributeEnabled(checkAttribute) || force) {
            Int32 shaderLocation = material->getShaderLocation(setAttribute);
            if (array->getGPUStorage()) {
                array->getGPUStorage()->enableAndSendToActiveShader(shaderLocation);
            }
        }
    }

    void MeshRenderer::disableShaderAttribute(WeakPointer<Mesh> mesh, WeakPointer<Material> material, StandardAttribute attribute,
                                              WeakPointer<AttributeArrayBase> array) {
        if (mesh->isAttributeEnabled(attribute)) {
            Int32 shaderLocation = material->getShaderLocation(attribute);
            WeakPointer<AttributeArrayGPUStorage> gpuStorage = array->getGPUStorage();
            if (gpuStorage) gpuStorage->disable(shaderLocation);
        }
    }

    void MeshRenderer::drawMesh(WeakPointer<Mesh> mesh) {
        if (mesh->isIndexed()) {
            Engine::instance()->getGraphicsSystem()->drawBoundVertexBuffer(mesh->getIndexCount(), mesh->getIndexBuffer());
        } else {
            Engine::instance()->getGraphicsSystem()->drawBoundVertexBuffer(mesh->getVertexCount());
        }
    }

    void MeshRenderer::testAndSetTexture2DWithInc(WeakPointer<Shader> shader, UInt32& textureSlot, Int32 shaderVarLoc, UInt32 textureID) {
        if (shaderVarLoc >= 0) {
            shader->setTexture2D(textureSlot, shaderVarLoc, textureID);
            textureSlot++;
        }
    }

    void MeshRenderer::testAndSetTextureCubeWithInc(WeakPointer<Shader> shader, UInt32& textureSlot, Int32 shaderVarLoc, UInt32 textureID) {
        if (shaderVarLoc >= 0) {
            shader->setTextureCube(textureSlot, shaderVarLoc, textureID);
            textureSlot++;
        }
    }
}