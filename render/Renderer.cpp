#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <ctime>

#include "../Engine.h"
#include "../common/Constants.h"
#include "Camera.h"
#include "Renderer.h"
#include "ViewDescriptor.h"
#include "RenderTarget.h"
#include "RenderTargetCube.h"
#include "RenderTarget2D.h"
#include "../math/Matrix4x4.h"
#include "../render/BaseRenderableContainer.h"
#include "../render/MeshRenderer.h"
#include "../render/RenderableContainer.h"
#include "../render/EngineRenderQueue.h"
#include "../particles/ParticleSystemRenderer.h"
#include "../scene/Scene.h"
#include "../scene/Skybox.h"
#include "../image/TextureAttr.h"
#include "../image/Texture.h"
#include "../image/Texture2D.h"
#include "../material/DepthOnlyMaterial.h"
#include "../material/NormalsMaterial.h"
#include "../material/PositionsMaterial.h"
#include "../material/SSAOMaterial.h"
#include "../material/SSAOBlurMaterial.h"
#include "../material/PositionsAndNormalsMaterial.h"
#include "../material/BasicColoredMaterial.h"
#include "../material/DistanceOnlyMaterial.h"
#include "../material/TonemapMaterial.h"
#include "../material/IrradianceRendererMaterial.h"
#include "../material/SpecularIBLPreFilteredRendererMaterial.h"
#include "../material/SpecularIBLBRDFRendererMaterial.h"
#include "../math/Matrix4x4.h"
#include "../math/Quaternion.h"
#include "../light/PointLight.h"
#include "../light/AmbientLight.h"
#include "../light/AmbientIBLLight.h"
#include "../light/LightPack.h"
#include "../geometry/Mesh.h"
#include "ReflectionProbe.h"
#include "RenderUtils.h"


namespace Core {

    Renderer::Renderer() {
        
    }

    Renderer::~Renderer() {
        if (this->perspectiveShadowMapCameraObject.isValid()) Engine::safeReleaseObject(this->perspectiveShadowMapCameraObject);
        if (this->orthoShadowMapCameraObject.isValid()) Engine::safeReleaseObject(this->orthoShadowMapCameraObject);
    }

    Bool Renderer::init() {
        if (!this->depthMaterial.isValid()) {
            this->depthMaterial = Engine::instance()->createMaterial<DepthOnlyMaterial>();
            this->depthMaterial->setLit(false);
        }
        if (!this->normalsMaterial.isValid()) {
            this->normalsMaterial = Engine::instance()->createMaterial<NormalsMaterial>();
            this->normalsMaterial->setLit(false);
            this->normalsMaterial->setConvertToViewSpace(true);
        }
        if (!this->positionsMaterial.isValid()) {
            this->positionsMaterial = Engine::instance()->createMaterial<PositionsMaterial>();
            this->positionsMaterial->setLit(false);
            this->positionsMaterial->setConvertToViewSpace(true);
        }
        if (!this->positionsNormalsMaterial.isValid()) {
            this->positionsNormalsMaterial = Engine::instance()->createMaterial<PositionsAndNormalsMaterial>();
            this->positionsNormalsMaterial->setLit(false);
            this->positionsNormalsMaterial->setConvertToViewSpace(true);
        }
        if (!this->distanceMaterial.isValid()) {
            this->distanceMaterial = Engine::instance()->createMaterial<DistanceOnlyMaterial>();
            this->distanceMaterial->setLit(false);
        }
        if (!this->tonemapMaterial.isValid()) {
            this->tonemapMaterial = Engine::instance()->createMaterial<TonemapMaterial>();
            this->tonemapMaterial->setExposure(1.0f);
            this->tonemapMaterial->setLit(false);
        }

        const Vector2u depthNormalsRenderTargetSize(Constants::EffectsBuffer2DSize, Constants::EffectsBuffer2DSize);
        TextureAttributes depthNormalsColorAttributes;
        depthNormalsColorAttributes.Format = TextureFormat::RGBA16F;
        depthNormalsColorAttributes.FilterMode = TextureFilter::Point;
        depthNormalsColorAttributes.MipLevels = 0;
        depthNormalsColorAttributes.WrapMode = TextureWrap::Clamp;
        TextureAttributes depthNormalsDepthAttributes;
        depthNormalsDepthAttributes.IsDepthTexture = true;
        this->depthNormalsRenderTarget = Engine::instance()->getGraphicsSystem()->createRenderTarget2D(true, true, false, depthNormalsColorAttributes,
                                                                                                       depthNormalsDepthAttributes, depthNormalsRenderTargetSize);

        const Vector2u depthPositionsRenderTargetSize(Constants::EffectsBuffer2DSize, Constants::EffectsBuffer2DSize);
        TextureAttributes depthPositionsColorAttributes;
        depthPositionsColorAttributes.Format = TextureFormat::RGBA16F;
        depthPositionsColorAttributes.FilterMode = TextureFilter::Point;
        depthPositionsColorAttributes.MipLevels = 0;
        depthPositionsColorAttributes.WrapMode = TextureWrap::Clamp;
        TextureAttributes depthPositionsDepthAttributes;
        depthPositionsDepthAttributes.IsDepthTexture = true;
        this->depthPositionsRenderTarget = Engine::instance()->getGraphicsSystem()->createRenderTarget2D(true, true, false, depthPositionsColorAttributes,
                                                                                                         depthPositionsDepthAttributes, depthPositionsRenderTargetSize);

        const Vector2u positionsNormalsRenderTargetSize(Constants::EffectsBuffer2DSize, Constants::EffectsBuffer2DSize);
        TextureAttributes positionsNormalsColorAttributes;
        positionsNormalsColorAttributes.Format = TextureFormat::RGBA16F;
        positionsNormalsColorAttributes.FilterMode = TextureFilter::Point;
        positionsNormalsColorAttributes.MipLevels = 0;
        positionsNormalsColorAttributes.WrapMode = TextureWrap::Clamp;
        TextureAttributes positionsNormalsDepthAttributes;
        positionsNormalsDepthAttributes.IsDepthTexture = true;
        this->positionsNormalsRenderTarget = Engine::instance()->getGraphicsSystem()->createRenderTarget2D(true, true, false, positionsNormalsColorAttributes,
                                                                                                           positionsNormalsDepthAttributes, positionsNormalsRenderTargetSize);
        this->positionsNormalsRenderTarget->addColorTexture(positionsNormalsColorAttributes);

        this->initializeSSAO();

        return true;
    }

    void Renderer::renderScene(WeakPointer<Scene> scene, WeakPointer<Material> overrideMaterial) {
        this->renderScene(scene->getRoot(), overrideMaterial);
    }

    void Renderer::renderScene(WeakPointer<Object3D> rootObject, WeakPointer<Material> overrideMaterial) {
        static std::vector<WeakPointer<Object3D>> objectList;
        static std::vector<WeakPointer<Camera>> cameraList;
        static std::vector<WeakPointer<Light>> lightList;
        static std::vector<WeakPointer<Light>> nonIBLLightList;
        static std::vector<WeakPointer<DirectionalLight>> directionalLightList;
        static std::vector<WeakPointer<PointLight>> pointLightList;
        static std::vector<WeakPointer<AmbientLight>> ambientLightList;
        static std::vector<WeakPointer<AmbientIBLLight>> ambientIBLLightList;
        static LightPack lightPack;
        static LightPack nonIBLLightPack;
        static std::vector<WeakPointer<ReflectionProbe>> reflectionProbeList;
        static std::vector<WeakPointer<Object3D>> renderProbeObjects;
        objectList.resize(0);
        cameraList.resize(0);
        lightList.resize(0);
        nonIBLLightList.resize(0);
        directionalLightList.resize(0);
        pointLightList.resize(0);
        ambientIBLLightList.resize(0);
        ambientLightList.resize(0);
        lightPack.clear();
        nonIBLLightPack.clear();
        reflectionProbeList.resize(0);
        renderProbeObjects.resize(0);

        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        this->collectSceneObjectsAndComputeTransforms(rootObject, objectList);
        this->collectSceneObjectComponents(objectList, cameraList, reflectionProbeList, nonIBLLightList,
                                           directionalLightList, pointLightList, ambientLightList, ambientIBLLightList, lightList);

        for (UInt32 i = 0; i < directionalLightList.size(); i++) {
            lightPack.addDirectionalLight(directionalLightList[i]);
            nonIBLLightPack.addDirectionalLight(directionalLightList[i]);
        }
        for (UInt32 i = 0; i < pointLightList.size(); i++) {
            lightPack.addPointLight(pointLightList[i]);
            nonIBLLightPack.addPointLight(pointLightList[i]);
        }
        for (UInt32 i = 0; i < ambientLightList.size(); i++) {
            lightPack.addAmbientLight(ambientLightList[i]);
            nonIBLLightPack.addAmbientLight(ambientLightList[i]);
        }
        for (UInt32 i = 0; i < ambientIBLLightList.size(); i++) lightPack.addAmbientIBLLight(ambientIBLLightList[i]);

        // TODO: Decide how to classify objects as either contributors to ambient light or not
        /*for (UInt32 i = 0; i < objectList.size(); i++) {
            WeakPointer<Object3D> object = objectList[i];
            if (object->isStatic()) renderProbeObjects.push_back(object);
        }*/

        this->renderReflectionProbes(reflectionProbeList, renderProbeObjects, lightPack, nonIBLLightPack);
        for (UInt32 i = 0; i < ambientIBLLightList.size(); i++) {
            ambientIBLLightList[i]->updateMapsFromReflectionProbe();
        }

        this->renderPointLightShadowMaps(pointLightList, objectList);
        for (auto camera : cameraList) {
            this->renderDirectionalLightShadowMaps(directionalLightList, objectList, camera);
        }

        for (auto camera : cameraList) {
            WeakPointer<Material> savedOverrideMaterial = camera->getOverrideMaterial();
            if (overrideMaterial.isValid()) camera->setOverrideMaterial(overrideMaterial);
            this->renderForCamera(camera, objectList, lightPack, true);
            if (overrideMaterial.isValid()) camera->setOverrideMaterial(savedOverrideMaterial);
        }
    }

    void Renderer::collectSceneObjectComponents(std::vector<WeakPointer<Object3D>>& sceneObjects, std::vector<WeakPointer<Camera>>& cameraList,
                                                std::vector<WeakPointer<ReflectionProbe>>& reflectionProbeList, std::vector<WeakPointer<Light>>& nonIBLLightList,
                                                std::vector<WeakPointer<DirectionalLight>>& directionalLightList, std::vector<WeakPointer<PointLight>>& pointLightList,
                                                std::vector<WeakPointer<AmbientLight>>& ambientLightList, std::vector<WeakPointer<AmbientIBLLight>>& ambientIBLLightList,
                                                std::vector<WeakPointer<Light>>& lightList) {
        for (WeakPointer<Object3D> object : sceneObjects) {
            WeakPointer<Camera> camera = object->getCamera();
            if (camera.isValid() && camera->isActive()) {
                cameraList.push_back(camera);
            }
            WeakPointer<ReflectionProbe> reflectionProbe = object->getReflectionProbe();
            if (reflectionProbe.isValid() && reflectionProbe->isActive()) {
                reflectionProbeList.push_back(reflectionProbe);
            }
            WeakPointer<Light> light = object->getLight();
            if (light.isValid() && light->isActive()) {
                LightType lightType = light->getType();
                if (lightType != LightType::AmbientIBL) {
                    nonIBLLightList.push_back(light);
                    if (lightType == LightType::Directional) directionalLightList.push_back(object->getDirectionalLight());
                    else if (lightType == LightType::Point) pointLightList.push_back(object->getPointLight());
                    else if (lightType == LightType::Ambient) ambientLightList.push_back(object->getAmbientLight());
                } else {
                    ambientIBLLightList.push_back(object->getAmbientIBLLight());
                }
                lightList.push_back(light);
            }
        }
    }

    void Renderer::renderSceneBasic(WeakPointer<Object3D> rootObject, WeakPointer<Camera> camera, Bool matchPhysicalPropertiesWithLighting) {
        static std::vector<WeakPointer<Object3D>> objectList;
        objectList.resize(0);

        Matrix4x4 baseTransformation;
        rootObject->getTransform().getAncestorWorldMatrix(baseTransformation);

        this->collectSceneObjectsAndComputeTransforms(rootObject, objectList, baseTransformation);
        this->renderForCamera(camera, objectList, matchPhysicalPropertiesWithLighting);
    }

    void Renderer::renderForCamera(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects, Bool matchPhysicalPropertiesWithLighting) {
        static LightPack lightPack;
        this->renderForCamera(camera, objects, lightPack, matchPhysicalPropertiesWithLighting);
    }

    void Renderer::renderForCamera(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects, const LightPack& lightPack,
                                   Bool matchPhysicalPropertiesWithLighting) {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        WeakPointer<RenderTarget> nextRenderTarget = camera->getRenderTarget();
        if (!nextRenderTarget.isValid()) {
            nextRenderTarget = graphics->getDefaultRenderTarget();
        }

        if (nextRenderTarget->isCube()) {
            this->renderForCubeCamera(camera, objects, lightPack, matchPhysicalPropertiesWithLighting);
        }
        else {

            // TODO: Decided if only static object should be involved in rendering SSAO texture
            /*static std::vector<WeakPointer<Object3D>> staticObjects;
            staticObjects.resize(0);
            for (UInt32 i = 0; i < objects.size(); i++) {
                WeakPointer<Object3D> object = objects[i];
                if (object->isStatic()) staticObjects.push_back(object);
            }*/

            WeakPointer<Texture2D> ssaoMap = WeakPointer<Texture2D>::nullPtr();
            if (camera->isSSAOEnabled()) {
                this->renderSSAO(camera, objects);
                ssaoMap = this->ssaoBlurMap;
            }

            this->renderForStandardCamera(camera, objects, lightPack, matchPhysicalPropertiesWithLighting, ssaoMap);
        }
    }

    void Renderer::renderForStandardCamera(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects, const LightPack& lightPack,
                                           Bool matchPhysicalPropertiesWithLighting, WeakPointer<Texture2D> ssaoMap) {
        ViewDescriptor viewDescriptor;
        this->getViewDescriptorForCamera(camera, viewDescriptor);
        viewDescriptor.ssaoMap = ssaoMap;
        viewDescriptor.ssaoEnabled = ssaoMap.isValid();
        renderForViewDescriptor(viewDescriptor, objects, lightPack, matchPhysicalPropertiesWithLighting);
    }

    void Renderer::renderForCubeCamera(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects,
                                       const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        ViewDescriptor viewDesc;
        for (UInt32 i = 0; i < 6; i++) {
            this->getViewDescriptorForCubeCamera(camera, (CubeFace)i, viewDesc);
            this->renderForViewDescriptor(viewDesc, objects, lightPack, matchPhysicalPropertiesWithLighting);
        }
    }

    void Renderer::renderForViewDescriptor(ViewDescriptor& viewDescriptor, std::vector<WeakPointer<Object3D>>& objectList, 
                                           const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        static RenderQueueManager renderQueueManager;
        renderQueueManager.clearAll();

        Int32 overrideRenderQueueID = viewDescriptor.overrideMaterial.isValid() ? viewDescriptor.overrideMaterial->getRenderQueueID() : -1;
        this->sortObjectsIntoRenderQueues(objectList, renderQueueManager, overrideRenderQueueID);
        this->renderForViewDescriptor(viewDescriptor, renderQueueManager, lightPack, matchPhysicalPropertiesWithLighting);
    }

    void Renderer::renderForViewDescriptor(ViewDescriptor& viewDescriptor, RenderQueueManager& renderQueueManager, 
                                           const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        WeakPointer<RenderTarget> currentRenderTarget = this->preRenderForViewDescriptor(viewDescriptor);

        UInt32 renderQueueCount = renderQueueManager.getRenderQueueCount();
        for (UInt32 q = 0; q < renderQueueCount; q++) {
            this->renderRenderList(viewDescriptor, renderQueueManager.getRenderQueue(q), lightPack, matchPhysicalPropertiesWithLighting);
        }
        this->postRenderForViewDescriptor(viewDescriptor, currentRenderTarget);
    }

    void Renderer::renderForViewDescriptor(ViewDescriptor& viewDescriptor, RenderList& renderList, 
                                           const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        WeakPointer<RenderTarget> currentRenderTarget = this->preRenderForViewDescriptor(viewDescriptor);
        this->renderRenderList(viewDescriptor, renderList, lightPack, matchPhysicalPropertiesWithLighting);
        this->postRenderForViewDescriptor(viewDescriptor, currentRenderTarget);
    }

    void Renderer::renderRenderList(ViewDescriptor& viewDescriptor, RenderList& renderList, 
                                    const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        for (UInt32 i = 0; i < renderList.getItemCount(); i++) {
            RenderItem& renderItem = renderList.getRenderItem(i);
            this->renderRenderItem(viewDescriptor, renderItem, lightPack, matchPhysicalPropertiesWithLighting);
        }
    }

    void Renderer::renderRenderItem(ViewDescriptor& viewDescriptor, RenderItem& renderItem, 
                                    const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        if (renderItem.isActive) {
            if (renderItem.meshRenderer.isValid()) {
                renderItem.meshRenderer->forwardRenderMesh(viewDescriptor, renderItem.mesh, renderItem.isStatic, lightPack, matchPhysicalPropertiesWithLighting);
            } else if(renderItem.particleSystemRenderer.isValid()) {
                renderItem.particleSystemRenderer->forwardRenderParticleSystem(viewDescriptor, renderItem.particleSystem, renderItem.isStatic, lightPack, matchPhysicalPropertiesWithLighting);
            } else if (renderItem.renderer.isValid()) {
                renderItem.renderer->forwardRenderObject(viewDescriptor, renderItem.renderable, renderItem.isStatic, lightPack, matchPhysicalPropertiesWithLighting);
            }
        }
    }

    WeakPointer<RenderTarget> Renderer::preRenderForViewDescriptor(ViewDescriptor& viewDescriptor) {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        WeakPointer<RenderTarget> currentRenderTarget = graphics->getCurrentRenderTarget();

        WeakPointer<RenderTarget> nextRenderTarget = viewDescriptor.indirectHDREnabled ? viewDescriptor.hdrRenderTarget : viewDescriptor.renderTarget;
        graphics->activateRenderTarget(nextRenderTarget);       
        this->setViewportAndMipLevelForRenderTarget(nextRenderTarget, viewDescriptor.cubeFace);

        this->clearActiveRenderTarget(viewDescriptor);
        this->renderSkybox(viewDescriptor);
        return currentRenderTarget;
    }

    void Renderer::postRenderForViewDescriptor(ViewDescriptor& viewDescriptor, WeakPointer<RenderTarget> currentRenderTarget) {
         WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        if (viewDescriptor.indirectHDREnabled) {
            this->tonemapMaterial->setToneMapType(viewDescriptor.hdrToneMapType);
            this->tonemapMaterial->setExposure(viewDescriptor.hdrExposure);
            this->tonemapMaterial->setGamma(viewDescriptor.hdrGamma);
            graphics->blit(viewDescriptor.hdrRenderTarget, viewDescriptor.renderTarget, viewDescriptor.cubeFace, this->tonemapMaterial, true);
        }

        graphics->activateRenderTarget(currentRenderTarget);
        this->setViewportAndMipLevelForRenderTarget(currentRenderTarget, -1);
    }

    void Renderer::cullRenderListForPointLight(RenderList& renderList, WeakPointer<PointLight> pointLight) {
        return;
        static Point3r pointLightPos;
        pointLightPos.set(0.0f, 0.0f, 0.0f);
        pointLight->getOwner()->getTransform().applyTransformationTo(pointLightPos);
        for (UInt32 i = 0; i < renderList.getItemCount(); i++) {
            RenderItem& renderItem = renderList.getRenderItem(i);
            if (renderItem.mesh) {
                if (!RenderUtils::isPointLightInRangeOfMesh(pointLightPos, pointLight->getRadius(), renderItem.mesh, renderItem.meshRenderer->getOwner())) {
                    renderItem.isActive = false;
                }
            }
        }
    }

    void Renderer::renderSkybox(ViewDescriptor& viewDescriptor) {
        static LightPack lightPack;
        if (viewDescriptor.skybox != nullptr) {
            WeakPointer<BaseObject3DRenderer> renderer = viewDescriptor.skybox->getSkyboxObject()->getBaseRenderer();
            if (renderer) {
                ViewDescriptor skyboxView = viewDescriptor;
                skyboxView.cameraTransformation.setTranslation(0.0f, 0.0f, 0.0f);
                skyboxView.inverseCameraTransformation.copy(skyboxView.cameraTransformation);
                skyboxView.inverseCameraTransformation.invert();
                skyboxView.transposedCameraTransformation.copy(skyboxView.cameraTransformation);
                skyboxView.transposedCameraTransformation.transpose();
                renderer->forwardRender(skyboxView, lightPack, true);
            }
        }
    }

    void Renderer::renderObjectDirect(WeakPointer<Object3D> object, WeakPointer<Camera> camera, Bool matchPhysicalPropertiesWithLighting) {
        static LightPack lightPack;
        this->renderObjectDirect(object, camera, lightPack, matchPhysicalPropertiesWithLighting);
    }

    void Renderer::renderObjectDirect(WeakPointer<Object3D> object, WeakPointer<Camera> camera,
                                      const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        ViewDescriptor viewDescriptor;
        this->getViewDescriptorForCamera(camera, viewDescriptor);
        this->renderObjectDirect(object, viewDescriptor, lightPack, matchPhysicalPropertiesWithLighting);
    }

    void Renderer::renderObjectDirect(WeakPointer<Object3D> object, ViewDescriptor& viewDescriptor,
                                      const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        WeakPointer<BaseRenderableContainer> baseRenderableContainer = object->getBaseRenderableContainer();
        if (baseRenderableContainer) {
            WeakPointer<BaseObject3DRenderer> renderer = object->getBaseRenderer();
            if (renderer) {
                renderer->forwardRender(viewDescriptor, lightPack, matchPhysicalPropertiesWithLighting);
            }
        }
    }

    void Renderer::renderDirectionalLightShadowMaps(const std::vector<WeakPointer<DirectionalLight>>& lights,
                                                    std::vector<WeakPointer<Object3D>>& objects, WeakPointer<Camera> renderCamera) {
        static std::vector<std::vector<WeakPointer<Object3D>>> toRenderDirectional;
        static std::vector<WeakPointer<DirectionalLight>> renderLights;
        static LightPack lightPack;
        static RenderList renderList;

        if (!this->orthoShadowMapCamera.isValid()) {
            this->orthoShadowMapCameraObject = Engine::instance()->createObject3D();
            this->orthoShadowMapCamera = Engine::instance()->createOrthographicCamera(orthoShadowMapCameraObject, 1.0f, -1.0f, -1.0f, 1.0f, PointLight::NearPlane, PointLight::FarPlane);
        }

        UInt32 lightCount = 0;
        renderLights.resize(0);
        for (auto light: lights) {
            if (!this->isShadowCastingCapableLight(light)) continue;
            if (lightCount >= toRenderDirectional.size()) {
                toRenderDirectional.emplace_back();
            }
            std::vector<WeakPointer<Object3D>>& renderObjects = toRenderDirectional[lightCount];
            renderObjects.resize(0);
            lightCount++;
            renderLights.push_back(light);
        }
        for (UInt32 i = 0; i < objects.size(); i++) {
            WeakPointer<Object3D> object = objects[i];
            WeakPointer<BaseObject3DRenderer> renderer = object->getBaseRenderer();
            if (renderer && renderer->castsShadows()) {
                UInt32 curLight = 0;
                for (auto light: renderLights) {
                    std::vector<WeakPointer<Object3D>>& renderObjects = toRenderDirectional[curLight];
                    renderObjects.push_back(object);
                    curLight++;
                }
            }
        }

        UInt32 curLight = 0;
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        for (auto directionalLight: renderLights) {
            if (directionalLight->getShadowsEnabled()) {
                this->depthMaterial->setFaceCullingEnabled(directionalLight->getFaceCullingEnabled());
                this->depthMaterial->setCullFace(directionalLight->getCullFace());
                std::vector<DirectionalLight::OrthoProjection>& projections = directionalLight->buildProjections(renderCamera);
                Matrix4x4 viewTrans = directionalLight->getOwner()->getTransform().getWorldMatrix();
                ViewDescriptor viewDesc;
                viewDesc.indirectHDREnabled = false;
                viewDesc.cubeFace = -1;
                viewDesc.overrideMaterial = this->depthMaterial;
                viewDesc.depthOutputOverride = DepthOutputOverride::Depth;
                std::vector<WeakPointer<Object3D>>& renderObjects = toRenderDirectional[curLight];
                renderList.clear();
                this->buildRenderListFromObjects(renderObjects, renderList);
                for (UInt32 i = 0; i < directionalLight->getCascadeCount(); i++) {
                    DirectionalLight::OrthoProjection& proj = projections[i];  
                    this->orthoShadowMapCamera->setDimensions(proj.top, proj.bottom, proj.left, proj.right);        
                    this->orthoShadowMapCamera->setNearAndFar(proj.near, proj.far);
                    this->getViewDescriptorTransformations(viewTrans, orthoShadowMapCamera->getProjectionMatrix(),
                                                           this->orthoShadowMapCamera->getAutoClearRenderBuffers(), viewDesc);
                    viewDesc.renderTarget = directionalLight->getShadowMap(i);
                    this->renderForViewDescriptor(viewDesc, renderList, lightPack, true);
                }
            }
            curLight++;
        }
    }

    void Renderer::renderPointLightShadowMaps(const std::vector<WeakPointer<PointLight>>& lights, std::vector<WeakPointer<Object3D>>& objects) {
        static std::vector<std::vector<WeakPointer<Object3D>>> toRenderPoint;
        static std::vector<WeakPointer<PointLight>> renderLights;
        static LightPack lightPack;
        static RenderList renderList;

        if (!this->perspectiveShadowMapCamera.isValid()) {
            this->perspectiveShadowMapCameraObject = Engine::instance()->createObject3D();
            this->perspectiveShadowMapCamera = Engine::instance()->createPerspectiveCamera(perspectiveShadowMapCameraObject, Math::PI / 2.0f, 1.0f, PointLight::NearPlane, PointLight::FarPlane);
        }

        UInt32 lightCount = 0;
        renderLights.resize(0);
        for (auto light: lights) {
            if (!this->isShadowCastingCapableLight(light)) continue;
            if (lightCount >= toRenderPoint.size()) {
                toRenderPoint.emplace_back();
            }
            std::vector<WeakPointer<Object3D>>& renderObjects = toRenderPoint[lightCount];
            renderObjects.resize(0);
            lightCount++;
            renderLights.push_back(light);
        }

        for (UInt32 i = 0; i < objects.size(); i++) {
            WeakPointer<Object3D> object = objects[i];
            WeakPointer<BaseObject3DRenderer> renderer = object->getBaseRenderer();
            if (renderer && renderer->castsShadows()) {
                UInt32 curLight = 0;
                for (auto light: renderLights) {
                    std::vector<WeakPointer<Object3D>>& renderObjects = toRenderPoint[curLight];
                    renderObjects.push_back(object);
                    curLight++;
                }
            }
        }

        UInt32 curLight = 0;
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        for (auto pointLight: renderLights) {
            if (pointLight->getShadowsEnabled()) {
                WeakPointer<RenderTarget> shadowMapRenderTarget = pointLight->getShadowMap();
                WeakPointer<Object3D> lightObject = pointLight->getOwner();
                Matrix4x4 lightTransform = lightObject->getTransform().getWorldMatrix();
                this->perspectiveShadowMapCameraObject->getTransform().getWorldMatrix().copy(lightTransform);
                this->perspectiveShadowMapCamera->setRenderTarget(shadowMapRenderTarget);
                Vector4u renderTargetDimensions = shadowMapRenderTarget->getViewport();
                this->perspectiveShadowMapCamera->setAspectRatioFromDimensions(renderTargetDimensions.z, renderTargetDimensions.w);
                this->perspectiveShadowMapCamera->setOverrideMaterial(this->distanceMaterial);
                this->perspectiveShadowMapCamera->setDepthOutputOverride(DepthOutputOverride::Distance);
                std::vector<WeakPointer<Object3D>>& renderObjects = toRenderPoint[curLight];
                renderList.clear();
                this->buildRenderListFromObjects(renderObjects, renderList);
                this->cullRenderListForPointLight(renderList, pointLight);
                ViewDescriptor viewDesc;
                for (UInt32 i = 0; i < 6; i++) {
                    this->getViewDescriptorForCubeCamera(this->perspectiveShadowMapCamera, (CubeFace)i, viewDesc);
                    this->renderForViewDescriptor(viewDesc, renderList, lightPack, true);
                }
            }
            curLight++;
        }
    }

    void Renderer::setViewportAndMipLevelForRenderTarget(WeakPointer<RenderTarget> renderTarget, Int16 cubeFace) {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        UInt32 targetMipLevel = renderTarget->getMipLevel();
        if (cubeFace >= 0)
            graphics->activateCubeRenderTargetSide((CubeTextureSide)cubeFace, targetMipLevel);
        else
            graphics->activateRenderTarget2DMipLevel(targetMipLevel);
        Vector4u mipLevelScaledViewport = renderTarget->getViewportForMipLevel(targetMipLevel);
        graphics->setViewport(mipLevelScaledViewport.x, mipLevelScaledViewport.y, mipLevelScaledViewport.z, mipLevelScaledViewport.w);

    }

    void Renderer::clearActiveRenderTarget(ViewDescriptor& viewDescriptor) {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        Bool clearColorBuffer = IntMaskUtil::isBitSetForMask(viewDescriptor.clearRenderBuffers, (UInt32)RenderBufferType::Color);
        Bool clearDepthBuffer = IntMaskUtil::isBitSetForMask(viewDescriptor.clearRenderBuffers, (UInt32)RenderBufferType::Depth);
        Bool clearStencilBuffer = IntMaskUtil::isBitSetForMask(viewDescriptor.clearRenderBuffers, (UInt32)RenderBufferType::Stencil);
        graphics->clearActiveRenderTarget(clearColorBuffer, clearDepthBuffer, clearStencilBuffer);
    }

    void Renderer::getViewDescriptorForCubeCamera(WeakPointer<Camera> camera, CubeFace cubeFace, ViewDescriptor& outDescriptor) {
        static bool initialized = false;
        static Matrix4x4 forward;
        static Matrix4x4 left;
        static Matrix4x4 right;
        static Matrix4x4 up;
        static Matrix4x4 down;
        static Matrix4x4 backward;
        static std::vector<Matrix4x4> orientations;
        if (!initialized) {
            initialized = true;
            forward.lookAt(Vector3r::Zero, Vector3r::Backward, Vector3r::Down);
            backward.lookAt(Vector3r::Zero, Vector3r::Forward, Vector3r::Down);
            up.lookAt(Vector3r::Zero, Vector3r::Up, Vector3r::Backward);
            down.lookAt(Vector3r::Zero, Vector3r::Down, Vector3r::Forward);
            left.lookAt(Vector3r::Zero, Vector3r::Left, Vector3r::Down);
            right.lookAt(Vector3r::Zero, Vector3r::Right, Vector3r::Down);
            orientations.push_back(forward);
            orientations.push_back(backward);
            orientations.push_back(up);
            orientations.push_back(down);
            orientations.push_back(left);
            orientations.push_back(right);
        }

        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ViewDescriptor baseViewDescriptor;
        this->getViewDescriptorForCamera(camera, baseViewDescriptor);
    
        ViewDescriptor viewDescriptor = baseViewDescriptor;
        Matrix4x4 cameraTransform = camera->getOwner()->getTransform().getWorldMatrix();
        cameraTransform.multiply(orientations[(UInt16)cubeFace]);
        this->getViewDescriptorTransformations(cameraTransform, camera->getProjectionMatrix(),
                                               camera->getAutoClearRenderBuffers(), viewDescriptor);
        viewDescriptor.cubeFace = (UInt16)cubeFace;
        viewDescriptor.overrideMaterial = camera->getOverrideMaterial();
        outDescriptor = viewDescriptor;
    }
                                    
    void Renderer::getViewDescriptorsForCubeCamera(WeakPointer<Camera> camera, ViewDescriptor& descForward, ViewDescriptor& descBackward,
                                                   ViewDescriptor& descUp, ViewDescriptor& descDown, ViewDescriptor& descLeft, ViewDescriptor& descRight) {
        this->getViewDescriptorForCubeCamera(camera, CubeFace::Forward, descForward);
        this->getViewDescriptorForCubeCamera(camera, CubeFace::Backward, descBackward);
        this->getViewDescriptorForCubeCamera(camera, CubeFace::Up, descUp);
        this->getViewDescriptorForCubeCamera(camera, CubeFace::Down, descDown);
        this->getViewDescriptorForCubeCamera(camera, CubeFace::Left, descLeft);
        this->getViewDescriptorForCubeCamera(camera, CubeFace::Right, descRight);
    }

    void Renderer::getViewDescriptorForCamera(WeakPointer<Camera> camera, ViewDescriptor& viewDescriptor) {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        WeakPointer<RenderTarget> cameraRenderTarget = camera->getRenderTarget();
        if (!cameraRenderTarget.isValid()) {
            cameraRenderTarget = graphics->getDefaultRenderTarget();
        }
        if (camera->isHDREnabled() && !cameraRenderTarget->isHDRCapable()) {
            Vector2u targetSize = cameraRenderTarget->getSize();
            WeakPointer<RenderTarget2D> cameraHDRRenderTarget = camera->getHDRRenderTarget();
            if (!cameraHDRRenderTarget.isValid() || 
                cameraHDRRenderTarget->getSize().x != targetSize.x ||
                cameraHDRRenderTarget->getSize().y != targetSize.y) {
                camera->buildHDRRenderTarget(targetSize);
            }
            viewDescriptor.renderTarget = cameraRenderTarget;
            viewDescriptor.hdrRenderTarget = camera->getHDRRenderTarget();
            viewDescriptor.indirectHDREnabled = true;
            viewDescriptor.hdrToneMapType = camera->getHDRToneMapType();
        }
        else {
            viewDescriptor.indirectHDREnabled = false;
            viewDescriptor.hdrRenderTarget = WeakPointer<RenderTarget2D>::nullPtr();
            viewDescriptor.renderTarget = cameraRenderTarget;
        }
        viewDescriptor.overrideMaterial = camera->getOverrideMaterial();
        viewDescriptor.depthOutputOverride = camera->getDepthOutputOverride();
        viewDescriptor.hdrExposure = camera->getHDRExposure();
        viewDescriptor.hdrGamma = camera->getHDRGamma();
        viewDescriptor.skybox = camera->isSkyboxEnabled() ? &camera->getSkybox() : nullptr;
        this->getViewDescriptorTransformations(camera->getOwner()->getTransform().getWorldMatrix(),
                                camera->getProjectionMatrix(), camera->getAutoClearRenderBuffers(), viewDescriptor);
        viewDescriptor.cameraPosition.set(0.0f, 0.0f, 0.0f);
        viewDescriptor.cubeFace = -1;
        viewDescriptor.cameraTransformation.transform(viewDescriptor.cameraPosition);
        viewDescriptor.ssaoEnabled = camera->isSSAOEnabled();
        viewDescriptor.ssaoMap = WeakPointer<Texture2D>::nullPtr();
        viewDescriptor.ssaoRadius = camera->getSSAORadius();
        viewDescriptor.ssaoBias = camera->getSSAOBias();
       
    }

    void Renderer::getViewDescriptorTransformations(const Matrix4x4& worldMatrix, const Matrix4x4& projectionMatrix,
                                     IntMask clearBuffers, ViewDescriptor& viewDescriptor) {
        viewDescriptor.projectionMatrix.copy(projectionMatrix);
        viewDescriptor.cameraTransformation.copy(worldMatrix);
        viewDescriptor.inverseCameraTransformation.copy(worldMatrix);
        viewDescriptor.inverseCameraTransformation.invert();
        viewDescriptor.transposedCameraTransformation.copy(viewDescriptor.cameraTransformation);
        viewDescriptor.transposedCameraTransformation.transpose();
        viewDescriptor.clearRenderBuffers = clearBuffers;
    }

    void Renderer::collectSceneObjectsAndComputeTransforms(WeakPointer<Scene> scene, std::vector<WeakPointer<Object3D>>& outObjects) {
        collectSceneObjectsAndComputeTransforms(scene->getRoot(), outObjects);
    }

    void Renderer::collectSceneObjectsAndComputeTransforms(WeakPointer<Object3D> object, std::vector<WeakPointer<Object3D>>& outObjects) {
        Matrix4x4 rootTransform;
        collectSceneObjectsAndComputeTransforms(object, outObjects, rootTransform);
    }

    void Renderer::collectSceneObjectsAndComputeTransforms(WeakPointer<Object3D> object, std::vector<WeakPointer<Object3D>>& outObjects, const Matrix4x4& curTransform) {

        if (!object->isActive()) return;
        Matrix4x4 nextTransform = curTransform;
        Transform& objTransform = object->getTransform();
        nextTransform.multiply(objTransform.getLocalMatrix());
        objTransform.getWorldMatrix().copy(nextTransform);
        outObjects.push_back(object);

        for (SceneObjectIterator<Object3D> itr = object->beginIterateChildren(); itr != object->endIterateChildren(); ++itr) {
            WeakPointer<Object3D> obj = *itr;
            this->collectSceneObjectsAndComputeTransforms(obj, outObjects, nextTransform);
        }
    }

    void Renderer::renderReflectionProbes(std::vector<WeakPointer<ReflectionProbe>>& reflectionProbeList, std::vector<WeakPointer<Object3D>> renderProbeObjects,
                                          const LightPack& lightPack, const LightPack& nonIBLLightPack) {
        static std::vector<WeakPointer<Object3D>> emptyObjectList;
        for (auto reflectionProbe : reflectionProbeList) {
            if (reflectionProbe->getNeedsFullUpdate() || reflectionProbe->getNeedsSpecularUpdate()) {

                this->renderPointLightShadowMaps(lightPack.getPointLights(), renderProbeObjects);
                this->renderDirectionalLightShadowMaps(lightPack.getDirectionalLights(), renderProbeObjects, reflectionProbe->getRenderCamera());

                Bool specularOnly = !reflectionProbe->getNeedsFullUpdate();

                this->renderReflectionProbe(reflectionProbe, specularOnly, emptyObjectList, nonIBLLightPack);
                if (!reflectionProbe->isSkyboxOnly()) {
                    if (reflectionProbe->getRenderWithPhysical()) {
                        this->renderReflectionProbe(reflectionProbe, specularOnly, renderProbeObjects, lightPack);
                    } else {
                        this->renderReflectionProbe(reflectionProbe, specularOnly, renderProbeObjects, nonIBLLightPack);
                    }
                }

                if (specularOnly) reflectionProbe->setNeedsSpecularUpdate(false);
                else reflectionProbe->setNeedsFullUpdate(false);
            }
        }
    }

    void Renderer::renderReflectionProbe(WeakPointer<ReflectionProbe> reflectionProbe, Bool specularOnly,
                                         std::vector<WeakPointer<Object3D>>& renderObjects, const LightPack& lightPack) {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        WeakPointer<Camera> probeCam = reflectionProbe->getRenderCamera();

        probeCam->setRenderTarget(reflectionProbe->getSceneRenderTarget());

        this->renderForCamera(probeCam, renderObjects, lightPack, true);
        reflectionProbe->getSceneRenderTarget()->getColorTexture()->updateMipMaps();

        if(!specularOnly) {
            probeCam->setRenderTarget(reflectionProbe->getIrradianceMapRenderTarget());
            WeakPointer<Material> savedOverrideMaterial = probeCam->getOverrideMaterial();
            probeCam->setOverrideMaterial(reflectionProbe->getIrradianceRendererMaterial());
            this->renderSceneBasic(reflectionProbe->getSkyboxObject(), probeCam, true);
            probeCam->setOverrideMaterial(savedOverrideMaterial);
        }
        
        WeakPointer<RenderTargetCube> specularIBLPreFilteredMap = reflectionProbe->getSpecularIBLPreFilteredMapRenderTarget();
        probeCam->setRenderTarget(specularIBLPreFilteredMap);
        WeakPointer<SpecularIBLPreFilteredRendererMaterial> specularIBLPreFilteredRendererMaterial = reflectionProbe->getSpecularIBLPreFilteredRendererMaterial();
        specularIBLPreFilteredRendererMaterial->setTextureResolution(specularIBLPreFilteredMap->getSize().x);
        for(UInt32 i = 0; i <= specularIBLPreFilteredMap->getMaxMipLevel(); i++) {
            specularIBLPreFilteredMap->setMipLevel(i);
            Real roughness = (Real)i / (Real)(specularIBLPreFilteredMap->getMaxMipLevel());
            specularIBLPreFilteredRendererMaterial->setRoughness(roughness);
            WeakPointer<Material> savedOverrideMaterial = probeCam->getOverrideMaterial();
            probeCam->setOverrideMaterial(specularIBLPreFilteredRendererMaterial);
            this->renderSceneBasic(reflectionProbe->getSkyboxObject(), probeCam, true);
            probeCam->setOverrideMaterial(savedOverrideMaterial);
        }

        WeakPointer<RenderTarget2D> specularIBLBRDFMap = reflectionProbe->getSpecularIBLBRDFMapRenderTarget();
        graphics->renderFullScreenQuad(specularIBLBRDFMap, -1, reflectionProbe->getSpecularIBLBRDFRendererMaterial());
        
        reflectionProbe->setNeedsFullUpdate(false);
    }

    void Renderer::renderSSAO(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects) {

        ViewDescriptor viewDescriptor;
        this->getViewDescriptorForCamera(camera, viewDescriptor);

        DepthOutputOverride saveDepthOutputOverride = viewDescriptor.depthOutputOverride;
        viewDescriptor.depthOutputOverride = DepthOutputOverride::Depth;
        this->renderPositionsAndNormals(viewDescriptor, objects);
        viewDescriptor.depthOutputOverride = saveDepthOutputOverride;

        this->ssaoMaterial->setViewPositions(this->depthPositionsRenderTarget->getColorTexture(0));
        this->ssaoMaterial->setViewNormals(this->depthNormalsRenderTarget->getColorTexture(0));
        this->ssaoMaterial->setRadius(viewDescriptor.ssaoRadius);
        this->ssaoMaterial->setBias(viewDescriptor.ssaoBias);
        Vector2u ssaoRenderTargetSize = this->ssaoRenderTarget->getSize();
        this->ssaoMaterial->setScreenWidth(ssaoRenderTargetSize.x);
        this->ssaoMaterial->setScreenHeight(ssaoRenderTargetSize.y);
        //this->ssaoMaterial->setViewPositions(this->positionsNormalsRenderTarget->getColorTexture(0));
        //this->ssaoMaterial->setViewNormals(this->positionsNormalsRenderTarget->getColorTexture(1));
        this->ssaoMaterial->setProjection(viewDescriptor.projectionMatrix);

        WeakPointer<RenderTarget> saveRenderTarget = viewDescriptor.renderTarget;
        WeakPointer<RenderTarget> saveHDRRenderTarget = viewDescriptor.hdrRenderTarget;
        Bool saveIndirectHDREnabled = viewDescriptor.indirectHDREnabled;
        WeakPointer<Material> saveOverrideMaterial = viewDescriptor.overrideMaterial;

        viewDescriptor.indirectHDREnabled = false;
        viewDescriptor.hdrRenderTarget = WeakPointer<RenderTarget2D>::nullPtr();
        viewDescriptor.renderTarget = this->ssaoRenderTarget;
        viewDescriptor.overrideMaterial = this->ssaoMaterial;

        Engine::instance()->getGraphicsSystem()->renderFullScreenQuad(this->ssaoRenderTarget, -1, this->ssaoMaterial);
        ssaoBlurMaterial->setSSAOInput(this->ssaoRenderTarget->getColorTexture(0));
        Engine::instance()->getGraphicsSystem()->renderFullScreenQuad(this->ssaoBlurRenderTarget, -1, this->ssaoBlurMaterial);

        viewDescriptor.indirectHDREnabled = saveIndirectHDREnabled;
        viewDescriptor.hdrRenderTarget = saveHDRRenderTarget;
        viewDescriptor.renderTarget = saveRenderTarget;
        viewDescriptor.overrideMaterial = saveOverrideMaterial;
    }

    WeakPointer<Texture2D> Renderer::getSSAOTexture() {
        this->ssaoBlurMap;
    }

    void Renderer::renderDepthAndNormals(ViewDescriptor& viewDescriptor, std::vector<WeakPointer<Object3D>>& objects) {
        static LightPack lightPack;

        WeakPointer<RenderTarget> saveRenderTarget = viewDescriptor.renderTarget;
        WeakPointer<RenderTarget> saveHDRRenderTarget = viewDescriptor.hdrRenderTarget;
        Bool saveIndirectHDREnabled = viewDescriptor.indirectHDREnabled;
        WeakPointer<Material> saveOverrideMaterial = viewDescriptor.overrideMaterial;
        Bool saveSSAOEnabled = viewDescriptor.ssaoEnabled;

        viewDescriptor.indirectHDREnabled = false;
        viewDescriptor.hdrRenderTarget = WeakPointer<RenderTarget2D>::nullPtr();
        viewDescriptor.renderTarget = this->depthNormalsRenderTarget;
        viewDescriptor.overrideMaterial = this->normalsMaterial;

        this->renderForViewDescriptor(viewDescriptor, objects, lightPack, false);
       
        viewDescriptor.indirectHDREnabled = saveIndirectHDREnabled;
        viewDescriptor.hdrRenderTarget = saveHDRRenderTarget;
        viewDescriptor.renderTarget = saveRenderTarget;
        viewDescriptor.overrideMaterial = saveOverrideMaterial;
    }

    void Renderer::renderPositionsAndNormals(ViewDescriptor& viewDescriptor, std::vector<WeakPointer<Object3D>>& objects) {
        static LightPack lightPack;

        WeakPointer<RenderTarget> saveRenderTarget = viewDescriptor.renderTarget;
        WeakPointer<RenderTarget> saveHDRRenderTarget = viewDescriptor.hdrRenderTarget;
        Bool saveIndirectHDREnabled = viewDescriptor.indirectHDREnabled;
        WeakPointer<Material> saveOverrideMaterial = viewDescriptor.overrideMaterial;
        Bool saveSSAOEnabled = viewDescriptor.ssaoEnabled;

        viewDescriptor.indirectHDREnabled = false;
        viewDescriptor.hdrRenderTarget = WeakPointer<RenderTarget2D>::nullPtr();

        viewDescriptor.renderTarget = this->depthNormalsRenderTarget;
        viewDescriptor.overrideMaterial = this->normalsMaterial;
        this->renderForViewDescriptor(viewDescriptor, objects, lightPack, false);

        viewDescriptor.renderTarget = this->depthPositionsRenderTarget;
        viewDescriptor.overrideMaterial = this->positionsMaterial;
        this->renderForViewDescriptor(viewDescriptor, objects, lightPack, false);

        /*viewDescriptor.renderTarget = this->positionsNormalsRenderTarget;
        viewDescriptor.overrideMaterial = this->positionsNormalsMaterial;
        this->renderForViewDescriptor(viewDescriptor, objects, lightPack, false);*/

        viewDescriptor.indirectHDREnabled = saveIndirectHDREnabled;
        viewDescriptor.hdrRenderTarget = saveHDRRenderTarget;
        viewDescriptor.renderTarget = saveRenderTarget;
        viewDescriptor.overrideMaterial = saveOverrideMaterial;
    }

    void Renderer::initializeSSAO() {

        std::uniform_real_distribution<Real> randomFloats(0.0f, 1.0f); // generates random floats between 0.0 and 1.0
        std::default_random_engine generator;

        // generate sample kernel
        for (UInt32 i = 0; i < Constants::SSAOSamples; ++i) {
            Vector3r sample(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator));
            sample.normalize();
            sample = sample * randomFloats(generator);
            Real scale = Real(i) / Constants::SSAOSamples;

            // scale samples s.t. they're more aligned to center of kernel
            Real t = scale * scale;
            scale =  (1.0 - t) * 0.1f + t ;  //lerp(0.1f, 1.0f, scale * scale);
            sample = sample * scale;
            this->ssaoKernel.push_back(sample);
        }

        // generate noise texture
        Real ssaoNoiseData[16 * 4];
        for (unsigned int i = 0; i < 16; i++) {
            UInt32 offset = i * 4;
            ssaoNoiseData[offset] = randomFloats(generator) * 2.0 - 1.0;
            ssaoNoiseData[offset + 1] = randomFloats(generator) * 2.0 - 1.0;
            ssaoNoiseData[offset + 2] = 0.0f;
            ssaoNoiseData[offset + 3] = 0.0f;
        }

        TextureAttributes noiseTextureAttributes;
        noiseTextureAttributes.Format = TextureFormat::RGBA32F;
        noiseTextureAttributes.FilterMode = TextureFilter::Point;
        noiseTextureAttributes.MipLevels = 0;
        noiseTextureAttributes.WrapMode = TextureWrap::Clamp;
        this->ssaoNoise = Engine::instance()->getGraphicsSystem()->createTexture2D(noiseTextureAttributes);
        this->ssaoNoise->buildFromData(4, 4, (Byte*)ssaoNoiseData);

        const Vector2u ssaoRenderTargetSize(Constants::EffectsBuffer2DSize, Constants::EffectsBuffer2DSize);
        TextureAttributes ssaoColorAttributes;
        ssaoColorAttributes.Format = TextureFormat::R32F;
        ssaoColorAttributes.FilterMode = TextureFilter::Point;
        ssaoColorAttributes.MipLevels = 0;
        ssaoColorAttributes.WrapMode = TextureWrap::Clamp;
        TextureAttributes ssaoDepthAttributes;
        ssaoDepthAttributes.IsDepthTexture = true;
        this->ssaoRenderTarget = Engine::instance()->getGraphicsSystem()->createRenderTarget2D(true, true, false, ssaoColorAttributes,
                                                                                               ssaoDepthAttributes, ssaoRenderTargetSize);
        this->ssaoBlurRenderTarget = Engine::instance()->getGraphicsSystem()->createRenderTarget2D(true, true, false, ssaoColorAttributes,
                                                                                               ssaoDepthAttributes, ssaoRenderTargetSize);
        this->ssaoBlurMap = WeakPointer<Texture>::dynamicPointerCast<Texture2D>(this->ssaoBlurRenderTarget->getColorTexture());
        if (!this->ssaoMaterial.isValid()) {
            this->ssaoMaterial = Engine::instance()->createMaterial<SSAOMaterial>();
            this->ssaoMaterial->setLit(false);
            this->ssaoMaterial->setSamples(this->ssaoKernel);
            this->ssaoMaterial->setNoise(this->ssaoNoise);
        }

        if (!this->ssaoBlurMaterial.isValid()) {
            this->ssaoBlurMaterial = Engine::instance()->createMaterial<SSAOBlurMaterial>();
            this->ssaoBlurMaterial->setLit(false);
        }
    }

    void Renderer::sortObjectsIntoRenderQueues(std::vector<WeakPointer<Object3D>>& objects, RenderQueueManager& renderQueueManager, Int32 overrideRenderQueueID) {
        for(UInt32 i = 0; i < objects.size(); i++) {
            WeakPointer<Object3D> object = objects[i];
            WeakPointer<BaseObject3DRenderer> renderer = object->getBaseRenderer();
            if (renderer.isValid()) {
                UInt32 renderQueueID = -1;
                if (overrideRenderQueueID >= 0) {
                    renderQueueID = (UInt32)overrideRenderQueueID;
                } else {
                    renderQueueID = renderer->getRenderQueueID();
                }
                WeakPointer<BaseRenderableContainer> renderableContainer = object->getBaseRenderableContainer();
                WeakPointer<MeshRenderer> meshRenderer = object->getMeshRenderer();
                WeakPointer<MeshContainer> meshContainer = object->getMeshContainer();
                WeakPointer<ParticleSystemRenderer> particleSystemRenderer = object->getParticleSystemRenderer();
                WeakPointer<ParticleSystem> particleSystem = object->getParticleSystem();
                if (meshRenderer.isValid() && meshContainer.isValid()) {
                    UInt32 renderableCount = meshContainer->getBaseRenderableCount();
                    for(UInt32 i = 0; i < renderableCount; i++) {
                        WeakPointer<Mesh> mesh = meshContainer->getRenderable(i);
                        renderQueueManager.addMeshToQueue(renderQueueID, meshRenderer, mesh, object->isStatic(), true);
                    }
                } if (particleSystemRenderer.isValid() && particleSystem.isValid()) {
                    renderQueueManager.addParticleSystemToQueue(renderQueueID, particleSystemRenderer, particleSystem, object->isStatic(), true);
                } else if (renderableContainer.isValid()) {
                    UInt32 renderableCount = renderableContainer->getBaseRenderableCount();
                    for(UInt32 i = 0; i < renderableCount; i++) {
                        WeakPointer<BaseRenderable> renderable = renderableContainer->getBaseRenderable(i);
                        renderQueueManager.addItemToQueue(renderQueueID, renderer, renderable, object->isStatic(), true);
                    }
                }
            }
        }
    }

    void Renderer::buildRenderListFromObjects(std::vector<WeakPointer<Object3D>>& objects, RenderList& renderList) {
        renderList.clear();
         for(UInt32 i = 0; i < objects.size(); i++) {
            WeakPointer<Object3D> object = objects[i];
            WeakPointer<BaseObject3DRenderer> renderer = object->getBaseRenderer();
            if (renderer.isValid()) {
                WeakPointer<BaseRenderableContainer> renderableContainer = object->getBaseRenderableContainer();
                WeakPointer<MeshContainer> meshContainer = object->getMeshContainer();
                WeakPointer<MeshRenderer> meshRenderer = object->getMeshRenderer();
                WeakPointer<ParticleSystemRenderer> particleSystemRenderer = object->getParticleSystemRenderer();
                WeakPointer<ParticleSystem> particleSystem = object->getParticleSystem();
                if (meshRenderer.isValid() && meshContainer.isValid()) {
                    UInt32 renderableCount = meshContainer->getBaseRenderableCount();
                    for(UInt32 i = 0; i < renderableCount; i++) {
                        WeakPointer<Mesh> mesh = meshContainer->getRenderable(i);
                        renderList.addMesh(meshRenderer, mesh, object->isStatic(), true);
                    }
                } if (particleSystemRenderer.isValid() && particleSystem.isValid()) {
                    renderList.addParticleSystem(particleSystemRenderer, particleSystem, object->isStatic(), true);
                } else if (renderableContainer.isValid()) {
                    UInt32 renderableCount = renderableContainer->getBaseRenderableCount();
                    for(UInt32 i = 0; i < renderableCount; i++) {
                        WeakPointer<BaseRenderable> renderable = renderableContainer->getBaseRenderable(i);
                        renderList.addItem(renderer, renderable, object->isStatic(), true);
                    }
                }
            }
        }
    }

    Bool Renderer::isShadowCastingCapableLight(WeakPointer<Light> light) {
        LightType lightType = light->getType();
        if (lightType == LightType::Ambient || lightType == LightType::Planar) {
            return false;
        }
        return true;
    }

    Bool Renderer::compareLights (WeakPointer<Light> a, WeakPointer<Light> b) { 
        return ((UInt32)a->getType() < (UInt32)b->getType()); 
    }

}
