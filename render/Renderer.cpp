#include <vector>
#include <algorithm>

#include "../Engine.h"
#include "../math/Matrix4x4.h"
#include "../render/BaseRenderableContainer.h"
#include "../render/MeshRenderer.h"
#include "../render/RenderableContainer.h"
#include "../scene/Scene.h"
#include "Camera.h"
#include "Renderer.h"
#include "ViewDescriptor.h"
#include "RenderTarget.h"
#include "RenderTargetCube.h"
#include "RenderTarget2D.h"
#include "../image/TextureAttr.h"
#include "../image/Texture.h"
#include "../material/DepthOnlyMaterial.h"
#include "../material/DistanceOnlyMaterial.h"
#include "../math/Matrix4x4.h"
#include "../math/Quaternion.h"
#include "../light/PointLight.h"


namespace Core {

    Renderer::Renderer() {
    }

    Renderer::~Renderer() {
    }

    Bool Renderer::init() {
        this->setAutoClearRenderBuffer(RenderBufferType::Color, true);
        this->setAutoClearRenderBuffer(RenderBufferType::Depth, true);
        this->setAutoClearRenderBuffer(RenderBufferType::Stencil, true);
        return true;
    }

    void Renderer::render(WeakPointer<Scene> scene, WeakPointer<Material> overrideMaterial) {
        this->render(scene->getRoot(), overrideMaterial);
    }

    void Renderer::render(WeakPointer<Object3D> rootObject, WeakPointer<Material> overrideMaterial) {
        if (!this->depthMaterial.isValid()) {
            this->depthMaterial = Engine::instance()->createMaterial<DepthOnlyMaterial>();
        }
        if (!this->distanceMaterial.isValid()) {
            this->distanceMaterial = Engine::instance()->createMaterial<DistanceOnlyMaterial>();
        }
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();

        std::vector<WeakPointer<Object3D>> objectList;
        std::vector<WeakPointer<Camera>> cameraList;
        std::vector<WeakPointer<Light>> lightList;

        Matrix4x4 curTransform;
        this->processScene(rootObject, curTransform, objectList, cameraList, lightList);
        std::sort(lightList.begin(), lightList.end(), Renderer::compareLights);

        this->renderShadowMaps(lightList, LightType::Point, objectList);
        for (auto camera : cameraList) {
            this->renderShadowMaps(lightList, LightType::Directional, objectList, camera);
            this->render(camera, objectList, lightList, overrideMaterial);
        }
    }

    void Renderer::renderBasic(WeakPointer<Object3D> rootObject, WeakPointer<Camera> camera, WeakPointer<Material> overrideMaterial) {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();

        std::vector<WeakPointer<Object3D>> objectList;
        std::vector<WeakPointer<Camera>> cameraList;
        std::vector<WeakPointer<Light>> lightList;

        Matrix4x4 curTransform;
        this->processScene(rootObject, curTransform, objectList, cameraList, lightList);
        std::sort(lightList.begin(), lightList.end(), Renderer::compareLights);

        this->render(camera, objectList, lightList, overrideMaterial);
    }

    void Renderer::render(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects, std::vector<WeakPointer<Light>>& lights, WeakPointer<Material> overrideMaterial) {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
         WeakPointer<RenderTarget> nextRenderTarget = camera->getRenderTarget();
        if (!nextRenderTarget.isValid()) {
            nextRenderTarget = graphics->getDefaultRenderTarget();
        }

        graphics->setRenderingToBufferEnabled(RenderBufferType::Color,
                                              camera->isRenderBufferEnabled(RenderBufferType::Color));
        graphics->setRenderingToBufferEnabled(RenderBufferType::Depth,
                                              camera->isRenderBufferEnabled(RenderBufferType::Depth));
        graphics->setRenderingToBufferEnabled(RenderBufferType::Stencil,
                                              camera->isRenderBufferEnabled(RenderBufferType::Stencil));

        RenderTargetCube * renderTargetCube = dynamic_cast<RenderTargetCube*>(nextRenderTarget.get());
        if (renderTargetCube != nullptr) {
            this->renderCube(camera, objects, lights, overrideMaterial);
        }
        else {
            this->renderStandard(camera, objects, lights, overrideMaterial);
        }
    }

    void Renderer::setAutoClearRenderBuffer(RenderBufferType type, Bool clear) {
        if (clear) {
            IntMaskUtil::setBitForMask(&this->autoClearRenderBuffers, (UInt32)type);
        }
        else {
            IntMaskUtil::clearBitForMask(&this->autoClearRenderBuffers, (UInt32)type);
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
            this->getViewDescriptor(cameraTransform, camera->getProjectionMatrix(), viewDescriptor);
            viewDescriptor.overrideMaterial = overrideMaterial;
            viewDescriptor.cubeFace = i;
            viewDescriptor.renderTarget = camera->getRenderTarget();
            render(viewDescriptor, objects, lights);
        }
    }

    void Renderer::render(const ViewDescriptor& viewDescriptor, std::vector<WeakPointer<Object3D>>& objectList, std::vector<WeakPointer<Light>>& lightList) {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        WeakPointer<RenderTarget> currentRenderTarget = graphics->getCurrentRenderTarget();
        Vector4u currentViewport = graphics->getViewport();

        WeakPointer<RenderTarget> nextRenderTarget = viewDescriptor.renderTarget; 
        graphics->activateRenderTarget(nextRenderTarget);
        if (viewDescriptor.cubeFace >= 0) {
            graphics->activateCubeRenderTargetSide((CubeTextureSide)viewDescriptor.cubeFace);
        }
        Vector2u renderTargetSize = nextRenderTarget->getSize();
        Vector4u viewport = nextRenderTarget->getViewport();
        graphics->setViewport(viewport.x, viewport.y, viewport.z, viewport.w);
        graphics->setClearColor(Color(0.0, 0.0, 0.0, 1.0));

        Bool clearColorBuffer = IntMaskUtil::isBitSetForMask(this->autoClearRenderBuffers, (UInt32)RenderBufferType::Color);
        Bool clearDepthBuffer = IntMaskUtil::isBitSetForMask(this->autoClearRenderBuffers, (UInt32)RenderBufferType::Depth);
        Bool clearStencilBuffer = IntMaskUtil::isBitSetForMask(this->autoClearRenderBuffers, (UInt32)RenderBufferType::Stencil);

        graphics->clearActiveRenderTarget(clearColorBuffer, clearDepthBuffer, clearStencilBuffer); 

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
        if (!perspectiveShadowMapCamera.isValid()) {
            perspectiveShadowMapCameraObject = Engine::instance()->createObject3D();
            perspectiveShadowMapCamera = Engine::instance()->createPerspectiveCamera(perspectiveShadowMapCameraObject, Math::PI / 2.0f, 1.0f, PointLight::NearPlane, PointLight::FarPlane);
            orthoShadowMapCameraObject = Engine::instance()->createObject3D();
            orthoShadowMapCamera = Engine::instance()->createOrthographicCamera(orthoShadowMapCameraObject, 1.0f, -1.0f, -1.0f, 1.0f, PointLight::NearPlane, PointLight::FarPlane);
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
                            this->render(perspectiveShadowMapCamera, objects, dummyLights, this->distanceMaterial);
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
                                this->getViewDescriptor(viewTrans, orthoShadowMapCamera->getProjectionMatrix(), viewDesc);
                                viewDesc.overrideMaterial = this->depthMaterial;
                                viewDesc.renderTarget = directionalLight->getShadowMap(i);
                                this->render(viewDesc, objects, dummyLights);
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
        this->getViewDescriptor(camera->getOwner()->getTransform().getWorldMatrix(),
                                camera->getProjectionMatrix(), viewDescriptor);
        viewDescriptor.renderTarget = cameraRenderTarget;
    }

    void Renderer::getViewDescriptor(const Matrix4x4& worldMatrix, 
                                              const Matrix4x4& projectionMatrix, ViewDescriptor& viewDescriptor) {
        viewDescriptor.projectionMatrix.copy(projectionMatrix);
        viewDescriptor.viewMatrix.copy(worldMatrix);
        viewDescriptor.viewInverseMatrix.copy(viewDescriptor.viewMatrix);
        viewDescriptor.viewInverseMatrix.invert();
        viewDescriptor.viewInverseTransposeMatrix.copy(viewDescriptor.viewInverseMatrix);
        viewDescriptor.viewInverseTransposeMatrix.transpose();
    }

    void Renderer::processScene(WeakPointer<Scene> scene, 
                                std::vector<WeakPointer<Object3D>>& outObjects, 
                                std::vector<WeakPointer<Camera>>& outCameras,
                                std::vector<WeakPointer<Light>>& outLights) {
        Matrix4x4 rootTransform;
        processScene(scene->getRoot(), rootTransform, outObjects, outCameras, outLights);
    }

    void Renderer::processScene(WeakPointer<Object3D> object, 
                                const Matrix4x4& curTransform, 
                                std::vector<WeakPointer<Object3D>>& outObjects,
                                std::vector<WeakPointer<Camera>>& outCameras, 
                                std::vector<WeakPointer<Light>>& outLights) {

        Matrix4x4 nextTransform = curTransform;
        Transform& objTransform = object->getTransform();
        nextTransform.multiply(objTransform.getLocalMatrix());
        objTransform.getWorldMatrix().copy(nextTransform);
        Matrix4x4& inverseWorld = objTransform.getInverseWorldMatrix();
        inverseWorld.copy(nextTransform);
        inverseWorld.invert();
        outObjects.push_back(object);

        for (SceneObjectIterator<Object3DComponent> compItr = object->beginIterateComponents(); compItr != object->endIterateComponents(); ++compItr) {
            // check if this component is a camera
            WeakPointer<Object3DComponent> comp = (*compItr);
            WeakPointer<Camera> cameraPtr = WeakPointer<Object3DComponent>::dynamicPointerCast<Camera>(comp);
            if (cameraPtr.isValid()) {
                outCameras.push_back(cameraPtr);
                continue;
            }
            
            WeakPointer<Light> lightPtr = WeakPointer<Object3DComponent>::dynamicPointerCast<Light>(comp);
            if (lightPtr.isValid()) {
                outLights.push_back(lightPtr);
                continue;
            }
        }

        for (SceneObjectIterator<Object3D> itr = object->beginIterateChildren(); itr != object->endIterateChildren(); ++itr) {
            WeakPointer<Object3D> obj = *itr;
            this->processScene(obj, nextTransform, outObjects, outCameras, outLights);
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
