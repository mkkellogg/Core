#include <vector>
#include <algorithm>

#include "../Engine.h"
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
#include "../scene/Scene.h"
#include "../scene/Skybox.h"
#include "../image/TextureAttr.h"
#include "../image/Texture.h"
#include "../material/DepthOnlyMaterial.h"
#include "../material/DistanceOnlyMaterial.h"
#include "../material/IrridianceRendererMaterial.h"
#include "../math/Matrix4x4.h"
#include "../math/Quaternion.h"
#include "../light/PointLight.h"
#include "ReflectionProbe.h"


namespace Core {

    Renderer::Renderer() {
        this->ambientLightMode = RenderState::AmbientLightMode::Basic;
    }

    Renderer::~Renderer() {
    }

    Bool Renderer::init() {
        return true;
    }

    void Renderer::setAmbientLighMode(RenderState::AmbientLightMode mode) {
        this->ambientLightMode = mode;
    }

    void Renderer::renderScene(WeakPointer<Scene> scene, WeakPointer<Material> overrideMaterial) {
        this->renderScene(scene->getRoot(), overrideMaterial);
    }

    void Renderer::renderScene(WeakPointer<Object3D> rootObject, WeakPointer<Material> overrideMaterial) {
        static std::vector<WeakPointer<Object3D>> objectList;
        static std::vector<WeakPointer<Camera>> cameraList;
        static std::vector<WeakPointer<Light>> lightList;
        static std::vector<WeakPointer<ReflectionProbe>> reflectionProbeList;
        objectList.resize(0);
        cameraList.resize(0);
        lightList.resize(0);
        reflectionProbeList.resize(0);

        if (!this->depthMaterial.isValid()) {
            this->depthMaterial = Engine::instance()->createMaterial<DepthOnlyMaterial>();
            this->depthMaterial->setLit(false);
        }
        if (!this->distanceMaterial.isValid()) {
            this->distanceMaterial = Engine::instance()->createMaterial<DistanceOnlyMaterial>();
            this->distanceMaterial->setLit(false);
        }
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();

        Matrix4x4 curTransform;
        this->processScene(rootObject, curTransform, objectList);

        for (WeakPointer<Object3D> object : objectList) {
            for (SceneObjectIterator<Object3DComponent> compItr = object->beginIterateComponents(); compItr != object->endIterateComponents(); ++compItr) {
                WeakPointer<Object3DComponent> comp = (*compItr);
                WeakPointer<Camera> camera = WeakPointer<Object3DComponent>::dynamicPointerCast<Camera>(comp);
                if (camera.isValid() && camera->isActive()) {
                    cameraList.push_back(camera);
                    continue;
                }
                WeakPointer<Light> light = WeakPointer<Object3DComponent>::dynamicPointerCast<Light>(comp);
                if (light.isValid() && light->isActive()) {
                    if (light->getType() == LightType::Ambient && this->ambientLightMode != RenderState::AmbientLightMode::Basic) continue;
                    if (light->getType() == LightType::AmbientIBL && this->ambientLightMode != RenderState::AmbientLightMode::ImageBased) continue;
                    lightList.push_back(light);
                    continue;
                }
                WeakPointer<ReflectionProbe> reflectionProbe = WeakPointer<Object3DComponent>::dynamicPointerCast<ReflectionProbe>(comp);
                if (reflectionProbe.isValid() && reflectionProbe->isActive()) {
                    reflectionProbeList.push_back(reflectionProbe);
                    continue;
                }
            }
        }

        RenderState::AmbientLightMode oldAmbientMode = this->ambientLightMode;
        this->setAmbientLighMode(RenderState::AmbientLightMode::Basic);
        for (auto reflectionProbe : reflectionProbeList) {
            if (reflectionProbe->getNeedsUpdate()) {
                WeakPointer<Camera> probeCam = reflectionProbe->getRenderCamera();
                probeCam->setRenderTarget(reflectionProbe->getSceneRenderTarget());
                this->renderShadowMaps(lightList, LightType::Directional, objectList, probeCam);
                this->render(probeCam, objectList, lightList, WeakPointer<Material>::nullPtr());
                probeCam->setRenderTarget(reflectionProbe->getIrridianceMap());
                this->renderObjectBasic(reflectionProbe->getSkyboxObject(), probeCam, reflectionProbe->getIrridianceRendererMaterial());
                reflectionProbe->setNeedsUpdate(false);
            }
        }
        this->setAmbientLighMode(oldAmbientMode);

        std::sort(lightList.begin(), lightList.end(), Renderer::compareLights);

        this->renderShadowMaps(lightList, LightType::Point, objectList);
        for (auto camera : cameraList) {
            this->renderShadowMaps(lightList, LightType::Directional, objectList, camera);
            this->render(camera, objectList, lightList, overrideMaterial);
        }
    }

    void Renderer::renderObjectBasic(WeakPointer<Object3D> rootObject, WeakPointer<Camera> camera, WeakPointer<Material> overrideMaterial) {
        static std::vector<WeakPointer<Object3D>> objectList;
        objectList.resize(0);

        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        Matrix4x4 baseTransformation;
        if (rootObject->getParent()) {
            Transform& parentTransform = rootObject->getParent()->getTransform();
            parentTransform.updateWorldMatrix();
            baseTransformation = parentTransform.getWorldMatrix();
        }
        this->processScene(rootObject, baseTransformation, objectList);
        this->render(camera, objectList, overrideMaterial);
    }

    void Renderer::render(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects, 
                          WeakPointer<Material> overrideMaterial) {
        std::vector<WeakPointer<Light>> lightList;   
        this->render(camera, objects, lightList, overrideMaterial);                 
    }

    void Renderer::render(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects, std::vector<WeakPointer<Light>>& lights, WeakPointer<Material> overrideMaterial) {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        WeakPointer<RenderTarget> nextRenderTarget = camera->getRenderTarget();
        if (!nextRenderTarget.isValid()) {
            nextRenderTarget = graphics->getDefaultRenderTarget();
        }

        RenderTargetCube * renderTargetCube = dynamic_cast<RenderTargetCube*>(nextRenderTarget.get());
        if (renderTargetCube != nullptr) {
            this->renderCube(camera, objects, lights, overrideMaterial);
        }
        else {
            this->renderStandard(camera, objects, lights, overrideMaterial);
        }
    }

    void Renderer::renderStandard(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects,
                                  std::vector<WeakPointer<Light>>& lights, WeakPointer<Material> overrideMaterial) {
        ViewDescriptor viewDescriptor;
        this->getViewDescriptorForCamera(camera, viewDescriptor);
        viewDescriptor.overrideMaterial = overrideMaterial;
        render(viewDescriptor, objects, lights);
    }

    void Renderer::renderCube(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects,
                    std::vector<WeakPointer<Light>>& lights, WeakPointer<Material> overrideMaterial) {

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
            Vector3r vup(0.0, 1.0, 0.0);
            Vector3r vbackward(0.0, 0.0, -1.0);
            Vector3r vfront(0.0, 0.0, 1.0);
            Vector3r vdown(0.0, -1.0, 0.0);
            Vector3r origin(0.0, 0.0, 0.0);
            forward.lookAt(origin, Vector3r(0.0f, 0.0f, 1.0f), vdown);
            orientations.push_back(forward);

            backward.lookAt(origin, Vector3r(0.0f, 0.0f, -1.0f), vdown);
            orientations.push_back(backward);

            up.lookAt(origin, Vector3r(0.0f, 1.0f, 0.0f), Vector3r(0.0, 0.0, 1.0f));
            orientations.push_back(up);

            down.lookAt(origin, Vector3r(0.0f, -1.0f, 0.0f), Vector3r(0.0, 0.0, -1.0f));
            orientations.push_back(down);
       
            left.lookAt(origin, Vector3r(-1.0f, 0.0f, 0.0f), vdown);
            orientations.push_back(left);

            right.lookAt(origin, Vector3r(1.0f, 0.0f, 0.0f), vdown);
            orientations.push_back(right);
        }

        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        for (unsigned int i = 0; i < 6; i++) {
            ViewDescriptor viewDescriptor;
            Matrix4x4 cameraTransform = camera->getOwner()->getTransform().getWorldMatrix();
            cameraTransform.multiply(orientations[i]);
            Skybox * skybox = camera->isSkyboxEnabled() ? &camera->getSkybox() : nullptr;
            this->getViewDescriptor(cameraTransform, camera->getProjectionMatrix(),
                                    camera->getAutoClearRenderBuffers(),
                                    skybox, viewDescriptor);
            viewDescriptor.overrideMaterial = overrideMaterial;
            viewDescriptor.cubeFace = i;
            viewDescriptor.renderTarget = camera->getRenderTarget();
            render(viewDescriptor, objects, lights);
        }
    }

    void Renderer::render(const ViewDescriptor& viewDescriptor, std::vector<WeakPointer<Object3D>>& objectList, std::vector<WeakPointer<Light>>& lightList) {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        WeakPointer<RenderTarget> currentRenderTarget = graphics->getCurrentRenderTarget();
        Vector4u currentViewport = currentRenderTarget->getViewport();

        WeakPointer<RenderTarget> nextRenderTarget = viewDescriptor.renderTarget; 
        graphics->activateRenderTarget(nextRenderTarget);
        if (viewDescriptor.cubeFace >= 0) {
            graphics->activateCubeRenderTargetSide((CubeTextureSide)viewDescriptor.cubeFace);
        }
        Vector2u renderTargetSize = nextRenderTarget->getSize();
        Vector4u viewport = nextRenderTarget->getViewport();
        graphics->setViewport(viewport.x, viewport.y, viewport.z, viewport.w);

        Bool clearColorBuffer = IntMaskUtil::isBitSetForMask(viewDescriptor.clearRenderBuffers, (UInt32)RenderBufferType::Color);
        Bool clearDepthBuffer = IntMaskUtil::isBitSetForMask(viewDescriptor.clearRenderBuffers, (UInt32)RenderBufferType::Depth);
        Bool clearStencilBuffer = IntMaskUtil::isBitSetForMask(viewDescriptor.clearRenderBuffers, (UInt32)RenderBufferType::Stencil);

        graphics->clearActiveRenderTarget(clearColorBuffer, clearDepthBuffer, clearStencilBuffer); 

        if (viewDescriptor.skybox != nullptr) {
            WeakPointer<BaseObjectRenderer> objectRenderer = viewDescriptor.skybox->getSkyboxObject()->getBaseRenderer();
            if (objectRenderer) {
                ViewDescriptor skyboxView = viewDescriptor;
                std::vector<WeakPointer<Light>> dummyLights;
                skyboxView.viewMatrix.setTranslation(0.0f, 0.0f, 0.0f);
                skyboxView.viewInverseMatrix.copy(skyboxView.viewMatrix);
                skyboxView.viewInverseMatrix.invert();
                skyboxView.viewInverseTransposeMatrix.copy(skyboxView.viewInverseMatrix);
                skyboxView.viewInverseTransposeMatrix.transpose();
                objectRenderer->forwardRender(skyboxView, dummyLights);
            }
        }

        for (auto object : objectList) {
            std::shared_ptr<Object3D> objectShared = object.lock();
            std::shared_ptr<BaseRenderableContainer> containerPtr = std::dynamic_pointer_cast<BaseRenderableContainer>(objectShared);
            if (containerPtr) {
                WeakPointer<BaseObjectRenderer> objectRenderer = containerPtr->getBaseRenderer();
                if (objectRenderer) {
                    objectRenderer->forwardRender(viewDescriptor, lightList);
                }
            }
        }

        graphics->activateRenderTarget(currentRenderTarget);
        graphics->setViewport(currentViewport.x, currentViewport.y, currentViewport.z, currentViewport.w);
    }

    void Renderer::renderShadowMaps(std::vector<WeakPointer<Light>>& lights, LightType lightType,
                                    std::vector<WeakPointer<Object3D>>& objects, WeakPointer<Camera> renderCamera) {

        static PersistentWeakPointer<Camera> perspectiveShadowMapCamera;
        static PersistentWeakPointer<Object3D> perspectiveShadowMapCameraObject;
        static PersistentWeakPointer<Camera> orthoShadowMapCamera;
        static PersistentWeakPointer<Object3D> orthoShadowMapCameraObject;
        static std::vector<WeakPointer<Object3D>> toRender;
        if (!perspectiveShadowMapCamera.isValid()) {
            perspectiveShadowMapCameraObject = Engine::instance()->createObject3D();
            perspectiveShadowMapCamera = Engine::instance()->createPerspectiveCamera(perspectiveShadowMapCameraObject, Math::PI / 2.0f, 1.0f, PointLight::NearPlane, PointLight::FarPlane);
            orthoShadowMapCameraObject = Engine::instance()->createObject3D();
            orthoShadowMapCamera = Engine::instance()->createOrthographicCamera(orthoShadowMapCameraObject, 1.0f, -1.0f, -1.0f, 1.0f, PointLight::NearPlane, PointLight::FarPlane);
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
                            perspectiveShadowMapCameraObject->getTransform().getWorldMatrix().copy(lightTransform);
                            Vector4u renderTargetDimensions = shadowMapRenderTarget->getViewport();
                            perspectiveShadowMapCamera->setRenderTarget(shadowMapRenderTarget);  
                            perspectiveShadowMapCamera->setAspectRatioFromDimensions(renderTargetDimensions.z, renderTargetDimensions.w);                     
                            this->render(perspectiveShadowMapCamera, toRender, dummyLights, this->distanceMaterial);
                        }
                    }
                    break;
                    case LightType::Directional:
                    {
                        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
                        WeakPointer<DirectionalLight> directionalLight = WeakPointer<Light>::dynamicPointerCast<DirectionalLight>(light);
                        if (directionalLight->getShadowsEnabled()) {
                            std::vector<DirectionalLight::OrthoProjection>& projections = directionalLight->buildProjections(renderCamera);
                            Matrix4x4 viewTrans = directionalLight->getOwner()->getTransform().getWorldMatrix();
                            for (UInt32 i = 0; i < directionalLight->getCascadeCount(); i++) {
                                DirectionalLight::OrthoProjection& proj = projections[i];  
                                orthoShadowMapCamera->setDimensions(proj.top, proj.bottom, proj.left, proj.right);        
                                orthoShadowMapCamera->setNearAndFar(proj.near, proj.far);

                                ViewDescriptor viewDesc;
                                this->getViewDescriptor(viewTrans, orthoShadowMapCamera->getProjectionMatrix(),
                                                        orthoShadowMapCamera->getAutoClearRenderBuffers(),
                                                        nullptr, viewDesc);
                                viewDesc.overrideMaterial = this->depthMaterial;
                                viewDesc.renderTarget = directionalLight->getShadowMap(i);
                                this->render(viewDesc, toRender, dummyLights);
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
        Skybox * skybox = camera->isSkyboxEnabled() ? &camera->getSkybox() : nullptr;
        this->getViewDescriptor(camera->getOwner()->getTransform().getWorldMatrix(),
                                camera->getProjectionMatrix(), camera->getAutoClearRenderBuffers(),
                                skybox, viewDescriptor);
        viewDescriptor.renderTarget = cameraRenderTarget;
        viewDescriptor.cameraPosition.set(0.0f, 0.0f, 0.0f);
        viewDescriptor.viewMatrix.transform(viewDescriptor.cameraPosition);
       
    }

    void Renderer::getViewDescriptor(const Matrix4x4& worldMatrix, const Matrix4x4& projectionMatrix,
                                     IntMask clearBuffers, Skybox* skybox, ViewDescriptor& viewDescriptor) {
        viewDescriptor.projectionMatrix.copy(projectionMatrix);
        viewDescriptor.viewMatrix.copy(worldMatrix);
        viewDescriptor.viewInverseMatrix.copy(viewDescriptor.viewMatrix);
        viewDescriptor.viewInverseMatrix.invert();
        viewDescriptor.viewInverseTransposeMatrix.copy(viewDescriptor.viewInverseMatrix);
        viewDescriptor.viewInverseTransposeMatrix.transpose();
        viewDescriptor.clearRenderBuffers = clearBuffers;
        viewDescriptor.skybox = skybox;
    }

    void Renderer::processScene(WeakPointer<Scene> scene, 
                                std::vector<WeakPointer<Object3D>>& outObjects) {
        Matrix4x4 rootTransform;
        processScene(scene->getRoot(), rootTransform, outObjects);
    }

    void Renderer::processScene(WeakPointer<Object3D> object, 
                                const Matrix4x4& curTransform, 
                                std::vector<WeakPointer<Object3D>>& outObjects) {

        Matrix4x4 nextTransform = curTransform;
        Transform& objTransform = object->getTransform();
        nextTransform.multiply(objTransform.getLocalMatrix());
        objTransform.getWorldMatrix().copy(nextTransform);
        Matrix4x4& inverseWorld = objTransform.getInverseWorldMatrix();
        inverseWorld.copy(nextTransform);
        inverseWorld.invert();
        outObjects.push_back(object);

        for (SceneObjectIterator<Object3D> itr = object->beginIterateChildren(); itr != object->endIterateChildren(); ++itr) {
            WeakPointer<Object3D> obj = *itr;
            this->processScene(obj, nextTransform, outObjects);
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
