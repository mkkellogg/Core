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
#include "../light/AmbientIBLLight.h"
#include "../geometry/Mesh.h"
#include "ReflectionProbe.h"


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
        static std::vector<WeakPointer<ReflectionProbe>> reflectionProbeList;
        static std::vector<WeakPointer<Object3D>> renderProbeObjects;
        objectList.resize(0);
        cameraList.resize(0);
        lightList.resize(0);
        nonIBLLightList.resize(0);
        reflectionProbeList.resize(0);
        renderProbeObjects.resize(0);

        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        this->collectSceneObjectsAndComputeTransforms(rootObject, objectList);
        this->collectSceneObjectComponents(objectList, cameraList, reflectionProbeList, nonIBLLightList, lightList);

        std::sort(lightList.begin(), lightList.end(), Renderer::compareLights);
        std::sort(nonIBLLightList.begin(), nonIBLLightList.end(), Renderer::compareLights);

        // TODO: Decide how to classify objects as either contributors to ambient light or not
        /*for (UInt32 i = 0; i < objectList.size(); i++) {
            WeakPointer<Object3D> object = objectList[i];
            if (object->isStatic()) renderProbeObjects.push_back(object);
        }*/

        this->renderReflectionProbes(reflectionProbeList, renderProbeObjects, nonIBLLightList, lightList);

        this->renderShadowMaps(lightList, LightType::Point, objectList);
        for (auto camera : cameraList) {
            this->renderShadowMaps(lightList, LightType::Directional, objectList, camera);
        }

        for (auto camera : cameraList) {
            WeakPointer<Material> savedOverrideMaterial = camera->getOverrideMaterial();
            if (overrideMaterial.isValid()) camera->setOverrideMaterial(overrideMaterial);
            this->render(camera, objectList, lightList, true);
            if (overrideMaterial.isValid()) camera->setOverrideMaterial(savedOverrideMaterial);
        }
    }

    void Renderer::collectSceneObjectComponents(std::vector<WeakPointer<Object3D>>& sceneObjects, std::vector<WeakPointer<Camera>>& cameraList,
                                                std::vector<WeakPointer<ReflectionProbe>>& reflectionProbeList, std::vector<WeakPointer<Light>>& nonIBLLightList,
                                                std::vector<WeakPointer<Light>>& lightList) {
        for (WeakPointer<Object3D> object : sceneObjects) {
            for (SceneObjectIterator<Object3DComponent> compItr = object->beginIterateComponents(); compItr != object->endIterateComponents(); ++compItr) {
                WeakPointer<Object3DComponent> comp = (*compItr);
                WeakPointer<Camera> camera = WeakPointer<Object3DComponent>::dynamicPointerCast<Camera>(comp);
                if (camera.isValid() && camera->isActive()) {
                    cameraList.push_back(camera);
                    continue;
                }
                WeakPointer<ReflectionProbe> reflectionProbe = WeakPointer<Object3DComponent>::dynamicPointerCast<ReflectionProbe>(comp);
                if (reflectionProbe.isValid() && reflectionProbe->isActive()) {
                    reflectionProbeList.push_back(reflectionProbe);
                    continue;
                }
            }
            for (SceneObjectIterator<Object3DComponent> compItr = object->beginIterateComponents(); compItr != object->endIterateComponents(); ++compItr) {
                WeakPointer<Object3DComponent> comp = (*compItr);
                WeakPointer<Light> light = WeakPointer<Object3DComponent>::dynamicPointerCast<Light>(comp);
                if (light.isValid() && light->isActive()) {
                    if (light->getType() == LightType::AmbientIBL) {
                        if (reflectionProbeList.size() > 0) {
                            WeakPointer<AmbientIBLLight> ambientIBLlight = WeakPointer<Object3DComponent>::dynamicPointerCast<AmbientIBLLight>(comp);
                            
                            WeakPointer<CubeTexture> irradianceMap = WeakPointer<Texture>::dynamicPointerCast<CubeTexture>(reflectionProbeList[0]->getIrradianceMap()->getColorTexture());
                            ambientIBLlight->setIrradianceMap(irradianceMap);
                            
                            WeakPointer<CubeTexture> specularIBLPreFilteredMap = WeakPointer<Texture>::dynamicPointerCast<CubeTexture>(reflectionProbeList[0]->getSpecularIBLPreFilteredMap()->getColorTexture());
                            ambientIBLlight->setSpecularIBLPreFilteredMap(specularIBLPreFilteredMap);
                            
                            WeakPointer<Texture2D> specularIBLBRDFMap = WeakPointer<Texture>::dynamicPointerCast<Texture2D>(reflectionProbeList[0]->getSpecularIBLBRDFMap()->getColorTexture());
                            ambientIBLlight->setSpecularIBLBRDFMap(specularIBLBRDFMap);
                        }
                        else continue;
                    }
                    else {
                        nonIBLLightList.push_back(light);
                    }
                    lightList.push_back(light);
                    continue;
                }
            }
        }
    }

    void Renderer::renderObjectBasic(WeakPointer<Object3D> rootObject, WeakPointer<Camera> camera, Bool matchPhysicalPropertiesWithLighting) {
        static std::vector<WeakPointer<Object3D>> objectList;
        objectList.resize(0);

        Matrix4x4 baseTransformation;
        rootObject->getTransform().getAncestorWorldMatrix(baseTransformation);

        this->collectSceneObjectsAndComputeTransforms(rootObject, objectList, baseTransformation);
        this->render(camera, objectList, matchPhysicalPropertiesWithLighting);
    }

    void Renderer::render(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects, Bool matchPhysicalPropertiesWithLighting) {
        static std::vector<WeakPointer<Light>> lightList;   
        this->render(camera, objects, lightList, matchPhysicalPropertiesWithLighting);                 
    }

    void Renderer::render(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects, std::vector<WeakPointer<Light>>& lights,
                          Bool matchPhysicalPropertiesWithLighting) {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        WeakPointer<RenderTarget> nextRenderTarget = camera->getRenderTarget();
        if (!nextRenderTarget.isValid()) {
            nextRenderTarget = graphics->getDefaultRenderTarget();
        }

        RenderTargetCube * renderTargetCube = dynamic_cast<RenderTargetCube*>(nextRenderTarget.get());
        if (renderTargetCube != nullptr) {
            this->renderCube(camera, objects, lights, matchPhysicalPropertiesWithLighting);
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
                ssaoMap = WeakPointer<Texture>::dynamicPointerCast<Texture2D>(this->ssaoBlurRenderTarget->getColorTexture());
            }

            this->renderStandard(camera, objects, lights, matchPhysicalPropertiesWithLighting, ssaoMap);
        }
    }

    void Renderer::renderStandard(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects,
                                  std::vector<WeakPointer<Light>>& lights, Bool matchPhysicalPropertiesWithLighting,
                                  WeakPointer<Texture2D> ssaoMap) {
        ViewDescriptor viewDescriptor;
        this->getViewDescriptorForCamera(camera, viewDescriptor);
        viewDescriptor.ssaoMap = ssaoMap;
        viewDescriptor.ssaoEnabled = ssaoMap.isValid();
        render(viewDescriptor, objects, lights, matchPhysicalPropertiesWithLighting);
    }

    void Renderer::renderCube(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects,
                    std::vector<WeakPointer<Light>>& lights, Bool matchPhysicalPropertiesWithLighting) {

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
        for (unsigned int i = 0; i < 6; i++) {
            ViewDescriptor viewDescriptor = baseViewDescriptor;
            Matrix4x4 cameraTransform = camera->getOwner()->getTransform().getWorldMatrix();
            cameraTransform.multiply(orientations[i]);
            this->getViewDescriptorTransformations(cameraTransform, camera->getProjectionMatrix(),
                                                   camera->getAutoClearRenderBuffers(), viewDescriptor);
            viewDescriptor.cubeFace = i;
            viewDescriptor.overrideMaterial = camera->getOverrideMaterial();
            render(viewDescriptor, objects, lights, matchPhysicalPropertiesWithLighting);
        }
    }

    void Renderer::render(ViewDescriptor& viewDescriptor, std::vector<WeakPointer<Object3D>>& objectList, 
                          std::vector<WeakPointer<Light>>& lightList, Bool matchPhysicalPropertiesWithLighting) {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        WeakPointer<RenderTarget> currentRenderTarget = graphics->getCurrentRenderTarget();

        WeakPointer<RenderTarget> nextRenderTarget = viewDescriptor.indirectHDREnabled ? viewDescriptor.hdrRenderTarget : viewDescriptor.renderTarget;
        graphics->activateRenderTarget(nextRenderTarget);       
        this->setViewportAndMipLevelForRenderTarget(nextRenderTarget, viewDescriptor.cubeFace);

        this->clearActiveRenderTarget(viewDescriptor);

        this->renderSkybox(viewDescriptor);
        renderQueueManager.clearAll();
        this->sortObjectsIntoRenderQueues(objectList, this->renderQueueManager, viewDescriptor);

        UInt32 renderQueueCount = this->renderQueueManager.getRenderQueueCount();
        for (UInt32 q = 0; q < renderQueueCount; q++) {
            RenderQueue& queue = this->renderQueueManager.getRenderQueue(q);
            UInt32 itemCount = queue.getItemCount();
            for (UInt32 i = 0; i < itemCount; i++) {
                RenderQueue::RenderItem& item = queue.getRenderItem(i);
                item.ObjectRenderer->forwardRender(viewDescriptor, lightList, matchPhysicalPropertiesWithLighting);
            }
        }
        /*for (auto object : objectList) {
            this->renderObjectDirect(object, viewDescriptor, lightList, matchPhysicalPropertiesWithLighting);
        }*/

        if (viewDescriptor.indirectHDREnabled) {
            this->tonemapMaterial->setToneMapType(viewDescriptor.hdrToneMapType);
            this->tonemapMaterial->setExposure(viewDescriptor.hdrExposure);
            this->tonemapMaterial->setGamma(viewDescriptor.hdrGamma);
            graphics->blit(viewDescriptor.hdrRenderTarget, viewDescriptor.renderTarget, viewDescriptor.cubeFace, this->tonemapMaterial, true);
        }

        graphics->activateRenderTarget(currentRenderTarget);
        this->setViewportAndMipLevelForRenderTarget(currentRenderTarget, -1);
    }

    void Renderer::clearActiveRenderTarget(ViewDescriptor& viewDescriptor) {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        Bool clearColorBuffer = IntMaskUtil::isBitSetForMask(viewDescriptor.clearRenderBuffers, (UInt32)RenderBufferType::Color);
        Bool clearDepthBuffer = IntMaskUtil::isBitSetForMask(viewDescriptor.clearRenderBuffers, (UInt32)RenderBufferType::Depth);
        Bool clearStencilBuffer = IntMaskUtil::isBitSetForMask(viewDescriptor.clearRenderBuffers, (UInt32)RenderBufferType::Stencil);
        graphics->clearActiveRenderTarget(clearColorBuffer, clearDepthBuffer, clearStencilBuffer); 

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

    void Renderer::renderSkybox(ViewDescriptor& viewDescriptor) {
        if (viewDescriptor.skybox != nullptr) {
            WeakPointer<BaseObjectRenderer> objectRenderer = viewDescriptor.skybox->getSkyboxObject()->getBaseRenderer();
            if (objectRenderer) {
                ViewDescriptor skyboxView = viewDescriptor;
                std::vector<WeakPointer<Light>> dummyLights;
                skyboxView.cameraTransformation.setTranslation(0.0f, 0.0f, 0.0f);
                skyboxView.inverseCameraTransformation.copy(skyboxView.cameraTransformation);
                skyboxView.inverseCameraTransformation.invert();
                skyboxView.transposedCameraTransformation.copy(skyboxView.cameraTransformation);
                skyboxView.transposedCameraTransformation.transpose();
                objectRenderer->forwardRender(skyboxView, dummyLights, true);
            }
        }
    }

    void Renderer::renderObjectDirect(WeakPointer<Object3D> object, WeakPointer<Camera> camera, Bool matchPhysicalPropertiesWithLighting) {
        static std::vector<WeakPointer<Light>> lightList;
        this->renderObjectDirect(object, camera, lightList, matchPhysicalPropertiesWithLighting);
    }

    void Renderer::renderObjectDirect(WeakPointer<Object3D> object, WeakPointer<Camera> camera,
                                     std::vector<WeakPointer<Light>>& lightList, Bool matchPhysicalPropertiesWithLighting) {
        ViewDescriptor viewDescriptor;
        this->getViewDescriptorForCamera(camera, viewDescriptor);
        this->renderObjectDirect(object, viewDescriptor, lightList, matchPhysicalPropertiesWithLighting);
    }

    void Renderer::renderObjectDirect(WeakPointer<Object3D> object, ViewDescriptor& viewDescriptor,
                            std::vector<WeakPointer<Light>>& lightList, Bool matchPhysicalPropertiesWithLighting) {
        std::shared_ptr<Object3D> objectShared = object.lock();
        std::shared_ptr<BaseRenderableContainer> containerPtr = std::dynamic_pointer_cast<BaseRenderableContainer>(objectShared);
        if (containerPtr) {
            WeakPointer<BaseObjectRenderer> objectRenderer = containerPtr->getBaseRenderer();
            if (objectRenderer) {
                objectRenderer->forwardRender(viewDescriptor, lightList, matchPhysicalPropertiesWithLighting);
            }
        }
    }

    void Renderer::renderShadowMaps(std::vector<WeakPointer<Light>>& lights, LightType lightType,
                                    std::vector<WeakPointer<Object3D>>& objects, WeakPointer<Camera> renderCamera) {

        static std::vector<WeakPointer<Object3D>> toRender;
        if (!this->perspectiveShadowMapCamera.isValid()) {
            this->perspectiveShadowMapCameraObject = Engine::instance()->createObject3D();
            this->perspectiveShadowMapCamera = Engine::instance()->createPerspectiveCamera(perspectiveShadowMapCameraObject, Math::PI / 2.0f, 1.0f, PointLight::NearPlane, PointLight::FarPlane);
            this->orthoShadowMapCameraObject = Engine::instance()->createObject3D();
            this->orthoShadowMapCamera = Engine::instance()->createOrthographicCamera(orthoShadowMapCameraObject, 1.0f, -1.0f, -1.0f, 1.0f, PointLight::NearPlane, PointLight::FarPlane);
        }

        toRender.resize(0);
        for (UInt32 i = 0; i < objects.size(); i++) {
            WeakPointer<Object3D> object = objects[i];
            std::shared_ptr<Object3D> objectShared = object.lock();
            std::shared_ptr<BaseRenderableContainer> containerPtr = std::dynamic_pointer_cast<BaseRenderableContainer>(objectShared);
            if (containerPtr) {
                WeakPointer<BaseObjectRenderer> objectRenderer = containerPtr->getBaseRenderer();
                if (objectRenderer && objectRenderer->castsShadows()) {
                    toRender.push_back(object);
                }
            }
        }

        std::vector<WeakPointer<Light>> dummyLights;
        for (auto light: lights) {
            LightType clightType = light->getType();
            if (clightType == lightType && isShadowCastingCapableLight(light)) {
                switch(lightType) {
                    case LightType::Point:
                    {
                        WeakPointer<PointLight> pointLight = WeakPointer<Light>::dynamicPointerCast<PointLight>(light);
                        if (pointLight->getShadowsEnabled()) {
                            WeakPointer<RenderTarget> shadowMapRenderTarget = pointLight->getShadowMap();
                            WeakPointer<Object3D> lightObject = light->getOwner();
                            Matrix4x4 lightTransform = lightObject->getTransform().getWorldMatrix();
                            this->perspectiveShadowMapCameraObject->getTransform().getWorldMatrix().copy(lightTransform);
                            Vector4u renderTargetDimensions = shadowMapRenderTarget->getViewport();
                            this->perspectiveShadowMapCamera->setRenderTarget(shadowMapRenderTarget);  
                            this->perspectiveShadowMapCamera->setAspectRatioFromDimensions(renderTargetDimensions.z, renderTargetDimensions.w);
                            ViewDescriptor viewDesc;
                            this->getViewDescriptorForCamera(this->perspectiveShadowMapCamera, viewDesc);
                            viewDesc.overrideMaterial = this->distanceMaterial;
                            viewDesc.depthOutputOverride = DepthOutputOverride::Depth;
                            this->render(viewDesc, toRender, dummyLights, true);
                        }
                    }
                    break;
                    case LightType::Directional:
                    {
                        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
                        WeakPointer<DirectionalLight> directionalLight = WeakPointer<Light>::dynamicPointerCast<DirectionalLight>(light);
                        if (directionalLight->getShadowsEnabled()) {
                            this->depthMaterial->setFaceCullingEnabled(directionalLight->getFaceCullingEnabled());
                            this->depthMaterial->setCullFace(directionalLight->getCullFace());
                            std::vector<DirectionalLight::OrthoProjection>& projections = directionalLight->buildProjections(renderCamera);
                            Matrix4x4 viewTrans = directionalLight->getOwner()->getTransform().getWorldMatrix();
                            for (UInt32 i = 0; i < directionalLight->getCascadeCount(); i++) {
                                DirectionalLight::OrthoProjection& proj = projections[i];  
                                this->orthoShadowMapCamera->setDimensions(proj.top, proj.bottom, proj.left, proj.right);        
                                this->orthoShadowMapCamera->setNearAndFar(proj.near, proj.far);

                                ViewDescriptor viewDesc;
                                viewDesc.indirectHDREnabled = false;
                                viewDesc.cubeFace = -1;
                                this->getViewDescriptorTransformations(viewTrans, orthoShadowMapCamera->getProjectionMatrix(),
                                                                       this->orthoShadowMapCamera->getAutoClearRenderBuffers(), viewDesc);
                                viewDesc.overrideMaterial = this->depthMaterial;
                                viewDesc.renderTarget = directionalLight->getShadowMap(i);
                                viewDesc.depthOutputOverride = DepthOutputOverride::Distance;
                                this->render(viewDesc, toRender, dummyLights, true);
                            }
                        }
                    }
                }
            }
        }
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
                                          std::vector<WeakPointer<Light>>& nonIBLLightList, std::vector<WeakPointer<Light>>& lightList) {
        static std::vector<WeakPointer<Object3D>> emptyObjectList;
        for (auto reflectionProbe : reflectionProbeList) {
            if (reflectionProbe->getNeedsFullUpdate() || reflectionProbe->getNeedsSpecularUpdate()) {

                this->renderShadowMaps(lightList, LightType::Point, renderProbeObjects);
                this->renderShadowMaps(lightList, LightType::Directional, renderProbeObjects, reflectionProbe->getRenderCamera());

                Bool specularOnly = !reflectionProbe->getNeedsFullUpdate();

                this->renderReflectionProbe(reflectionProbe, specularOnly, emptyObjectList, nonIBLLightList);
                if (!reflectionProbe->isSkyboxOnly()) {
                    if (reflectionProbe->getRenderWithPhysical()) {
                        this->renderReflectionProbe(reflectionProbe, specularOnly, renderProbeObjects, lightList);
                    } else {
                        this->renderReflectionProbe(reflectionProbe, specularOnly, renderProbeObjects, nonIBLLightList);
                    }
                }

                if (specularOnly) reflectionProbe->setNeedsSpecularUpdate(false);
                else reflectionProbe->setNeedsFullUpdate(false);
            }
        }
    }

    void Renderer::renderReflectionProbe(WeakPointer<ReflectionProbe> reflectionProbe, Bool specularOnly,
                                         std::vector<WeakPointer<Object3D>>& renderObjects, std::vector<WeakPointer<Light>>& renderLights) {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        WeakPointer<Camera> probeCam = reflectionProbe->getRenderCamera();

        probeCam->setRenderTarget(reflectionProbe->getSceneRenderTarget());

        this->render(probeCam, renderObjects, renderLights, true);
        reflectionProbe->getSceneRenderTarget()->getColorTexture()->updateMipMaps();

        if(!specularOnly) {
            probeCam->setRenderTarget(reflectionProbe->getIrradianceMap());
            WeakPointer<Material> savedOverrideMaterial = probeCam->getOverrideMaterial();
            probeCam->setOverrideMaterial(reflectionProbe->getIrradianceRendererMaterial());
            this->renderObjectBasic(reflectionProbe->getSkyboxObject(), probeCam);
            probeCam->setOverrideMaterial(savedOverrideMaterial);
        }
        
        WeakPointer<RenderTargetCube> specularIBLPreFilteredMap = reflectionProbe->getSpecularIBLPreFilteredMap();
        probeCam->setRenderTarget(specularIBLPreFilteredMap);
        WeakPointer<SpecularIBLPreFilteredRendererMaterial> specularIBLPreFilteredRendererMaterial = reflectionProbe->getSpecularIBLPreFilteredRendererMaterial();
        specularIBLPreFilteredRendererMaterial->setTextureResolution(specularIBLPreFilteredMap->getSize().x);
        for(UInt32 i = 0; i <= specularIBLPreFilteredMap->getMaxMipLevel(); i++) {
            specularIBLPreFilteredMap->setMipLevel(i);
            Real roughness = (Real)i / (Real)(specularIBLPreFilteredMap->getMaxMipLevel());
            specularIBLPreFilteredRendererMaterial->setRoughness(roughness);
            WeakPointer<Material> savedOverrideMaterial = probeCam->getOverrideMaterial();
            probeCam->setOverrideMaterial(specularIBLPreFilteredRendererMaterial);
            this->renderObjectBasic(reflectionProbe->getSkyboxObject(), probeCam);
            probeCam->setOverrideMaterial(savedOverrideMaterial);
        }

        WeakPointer<RenderTarget2D> specularIBLBRDFMap = reflectionProbe->getSpecularIBLBRDFMap();
        graphics->renderFullScreenQuad(specularIBLBRDFMap, -1, reflectionProbe->getSpecularIBLBRDFRendererMaterial());
        
        reflectionProbe->setNeedsFullUpdate(false);
    }

    void Renderer::renderSSAO(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects) {
        static std::vector<WeakPointer<Light>> emptyLightList;

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
        WeakPointer<Texture2D> tex2D = WeakPointer<Texture>::dynamicPointerCast<Texture2D>(this->ssaoBlurRenderTarget->getColorTexture());
        //WeakPointer<Texture2D> tex2D = WeakPointer<Texture>::dynamicPointerCast<Texture2D>(this->depthPositionsRenderTarget->getColorTexture(0));
        return tex2D;
    }

    void Renderer::renderDepthAndNormals(ViewDescriptor& viewDescriptor, std::vector<WeakPointer<Object3D>>& objects) {
        static std::vector<WeakPointer<Light>> emptyLightList;

        WeakPointer<RenderTarget> saveRenderTarget = viewDescriptor.renderTarget;
        WeakPointer<RenderTarget> saveHDRRenderTarget = viewDescriptor.hdrRenderTarget;
        Bool saveIndirectHDREnabled = viewDescriptor.indirectHDREnabled;
        WeakPointer<Material> saveOverrideMaterial = viewDescriptor.overrideMaterial;
        Bool saveSSAOEnabled = viewDescriptor.ssaoEnabled;

        viewDescriptor.indirectHDREnabled = false;
        viewDescriptor.hdrRenderTarget = WeakPointer<RenderTarget2D>::nullPtr();
        viewDescriptor.renderTarget = this->depthNormalsRenderTarget;
        viewDescriptor.overrideMaterial = this->normalsMaterial;

        this->render(viewDescriptor, objects, emptyLightList, false);
       
        viewDescriptor.indirectHDREnabled = saveIndirectHDREnabled;
        viewDescriptor.hdrRenderTarget = saveHDRRenderTarget;
        viewDescriptor.renderTarget = saveRenderTarget;
        viewDescriptor.overrideMaterial = saveOverrideMaterial;
    }

    void Renderer::renderPositionsAndNormals(ViewDescriptor& viewDescriptor, std::vector<WeakPointer<Object3D>>& objects) {
        static std::vector<WeakPointer<Light>> emptyLightList;

        WeakPointer<RenderTarget> saveRenderTarget = viewDescriptor.renderTarget;
        WeakPointer<RenderTarget> saveHDRRenderTarget = viewDescriptor.hdrRenderTarget;
        Bool saveIndirectHDREnabled = viewDescriptor.indirectHDREnabled;
        WeakPointer<Material> saveOverrideMaterial = viewDescriptor.overrideMaterial;
        Bool saveSSAOEnabled = viewDescriptor.ssaoEnabled;

        viewDescriptor.indirectHDREnabled = false;
        viewDescriptor.hdrRenderTarget = WeakPointer<RenderTarget2D>::nullPtr();

        viewDescriptor.renderTarget = this->depthNormalsRenderTarget;
        viewDescriptor.overrideMaterial = this->normalsMaterial;
        this->render(viewDescriptor, objects, emptyLightList, false);

        viewDescriptor.renderTarget = this->depthPositionsRenderTarget;
        viewDescriptor.overrideMaterial = this->positionsMaterial;
        this->render(viewDescriptor, objects, emptyLightList, false);

        /*viewDescriptor.renderTarget = this->positionsNormalsRenderTarget;
        viewDescriptor.overrideMaterial = this->positionsNormalsMaterial;
        this->render(viewDescriptor, objects, emptyLightList, false);*/

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

    void Renderer::sortObjectsIntoRenderQueues(std::vector<WeakPointer<Object3D>>& objects, RenderQueueManager& renderQueueManager, ViewDescriptor& viewDescriptor) {
        for(UInt32 i = 0; i < objects.size(); i++) {
            WeakPointer<Object3D> object= objects[i];
            std::shared_ptr<Object3D> objectShared = object.lock();
            std::shared_ptr<BaseRenderableContainer> containerPtr = std::dynamic_pointer_cast<BaseRenderableContainer>(objectShared);
            if (containerPtr) {
                WeakPointer<BaseObjectRenderer> objectRenderer = containerPtr->getBaseRenderer();
                if (objectRenderer.isValid()) {
                    UInt32 renderQueueID = (UInt32)EngineRenderQueue::Geometry;
                    if (viewDescriptor.overrideMaterial.isValid()) {
                        renderQueueID = viewDescriptor.overrideMaterial->getRenderQueueID();
                    } else {
                        renderQueueID = objectRenderer->getRenderQueueID();
                    }
                    renderQueueManager.addItemToQueue(renderQueueID, objectRenderer);
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
