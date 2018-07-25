#include <vector>

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
        return true;
    }

    void Renderer::render(WeakPointer<Scene> scene) {
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
        this->processScene(scene, objectList, cameraList, lightList);

        this->renderShadowMaps(lightList, LightType::Point, objectList);
        for (auto camera : cameraList) {
            this->renderShadowMaps(lightList, LightType::Directional, objectList, camera);
            this->render(camera, objectList, lightList);
        }
    }

    void Renderer::render(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects, std::vector<WeakPointer<Light>>& lights, WeakPointer<Material> overrideMaterial) {
        
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
        WeakPointer<RenderTarget> currentRenderTarget = graphics->getCurrentRenderTarget();
        Vector4u currentViewport = graphics->getViewport();
        
        WeakPointer<RenderTarget> nextRenderTarget = camera->getRenderTarget();
        if (!nextRenderTarget.isValid()) {
            nextRenderTarget = graphics->getDefaultRenderTarget();
        }




        Vector2u nextSize = nextRenderTarget->getSize();
        Vector4u nextViewport = nextRenderTarget->getViewport();
        graphics->setViewport(nextViewport.x, nextViewport.y, nextViewport.z, nextViewport.w);
        graphics->activateRenderTarget(nextRenderTarget);
    
        RenderTargetCube * renderTargetCube = dynamic_cast<RenderTargetCube*>(nextRenderTarget.get());
        if (renderTargetCube != nullptr) {
            for (unsigned int i = 0; i < 6; i++) {
                graphics->activateCubeRenderTargetSide((CubeTextureSide)i);
                ViewDescriptor viewDescriptor;
                Matrix4x4 cameraTransform = camera->getOwner()->getTransform().getWorldMatrix();
                cameraTransform.multiply(orientations[i]);
                this->getViewDescriptorForCamera(cameraTransform, camera->getProjectionMatrix(), viewDescriptor);
                viewDescriptor.overrideMaterial = overrideMaterial;
                render(viewDescriptor, objects, lights);
            }
        }
        else {  
            ViewDescriptor viewDescriptor;
            this->getViewDescriptorForCamera(camera, viewDescriptor);
            viewDescriptor.overrideMaterial = overrideMaterial;
            render(viewDescriptor, objects, lights);
        }



        graphics->activateRenderTarget(currentRenderTarget);
        graphics->setViewport(currentViewport.x, currentViewport.y, currentViewport.z, currentViewport.w);
    }

    void Renderer::render(const ViewDescriptor& viewDescriptor, std::vector<WeakPointer<Object3D>>& objectList, std::vector<WeakPointer<Light>>& lightList) {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        graphics->setClearColor(Color(0.0, 0.0, 0.0, 1.0));
        graphics->clearActiveRenderTarget(true, true, true); 
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

    }

    void Renderer::renderShadowMaps(std::vector<WeakPointer<Light>>& lights, LightType lightType,
                                    std::vector<WeakPointer<Object3D>>& objects, WeakPointer<Camera> renderCamera) {
        static PersistentWeakPointer<Camera> perspectiveShadowMapCamera;
        static PersistentWeakPointer<Object3D> perspectiveShadowMapCameraObject;
        static PersistentWeakPointer<Camera> orthoShadowMapCamera;
        static PersistentWeakPointer<Object3D> orthoShadowMapCameraObject;
        if (!perspectiveShadowMapCamera.isValid()) {
            perspectiveShadowMapCameraObject = Engine::instance()->createObject3D();
            perspectiveShadowMapCamera = Engine::instance()->createPerspectiveCamera(perspectiveShadowMapCameraObject, Math::PI / 2.0f, 1.0f, 0.1f, 100);
            orthoShadowMapCameraObject = Engine::instance()->createObject3D();
            orthoShadowMapCamera = Engine::instance()->createOrthographicCamera(orthoShadowMapCameraObject, 1.0f, -1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
        }

        std::vector<WeakPointer<Light>> dummyLights;
        for (auto light: lights) {
            if (isShadowCastingCapableLight(light)) {
                LightType clightType = light->getType();
                if (clightType == lightType) {
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
                                //viewTrans.invert();
                                for (UInt32 i = 0; i < directionalLight->getCascadeCount(); i++) {
                                    DirectionalLight::OrthoProjection& proj = projections[i];  

                                    orthoShadowMapCameraObject->getTransform().getWorldMatrix().copy(viewTrans);
                                  //  orthoShadowMapCameraObject->getTransform().getWorldMatrix().setIdentity();
                                   /* Vector3r dir = Vector3r::Forward;
                                    viewTrans.transform(dir);
                                    orthoShadowMapCameraObject->getTransform().lookAt(Point3r(dir.x, dir.y, dir.z));
                                    orthoShadowMapCameraObject->getTransform().getWorldMatrix().copy(orthoShadowMapCameraObject->getTransform().getConstLocalMatrix());*/
                                    orthoShadowMapCamera->setDimensions(proj.top, proj.bottom, proj.left, proj.right);        
                                    orthoShadowMapCamera->setNearAndFar(proj.near, proj.far);
                                    WeakPointer<RenderTarget> shadowMapRenderTarget = directionalLight->getShadowMap(i);
                                    WeakPointer<RenderTarget> currentRenderTarget = graphics->getCurrentRenderTarget();
                                    graphics->activateRenderTarget(shadowMapRenderTarget);
                                    orthoShadowMapCamera->setRenderTarget(shadowMapRenderTarget); 
                                    Vector2u renderTargetSize = shadowMapRenderTarget->getSize();
                                    graphics->setViewport(0, 0, renderTargetSize.x, renderTargetSize.y);
                                    ViewDescriptor viewDesc;
                                    this->getViewDescriptorForCamera(orthoShadowMapCameraObject->getTransform().getWorldMatrix(),
                                                                     orthoShadowMapCamera->getProjectionMatrix(), viewDesc);
                                    viewDesc.overrideMaterial = this->depthMaterial;
                                    this->render(viewDesc, objects, dummyLights);
                                    graphics->activateRenderTarget(currentRenderTarget);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    void Renderer::getViewDescriptorForCamera(WeakPointer<Camera> camera, ViewDescriptor& viewDescriptor) {
        this->getViewDescriptorForCamera(camera->getOwner()->getTransform().getWorldMatrix(), camera->getProjectionMatrix(), viewDescriptor);
    }

    void Renderer::getViewDescriptorForCamera(const Matrix4x4& worldMatrix, const Matrix4x4& projectionMatrix, ViewDescriptor& viewDescriptor) {
        viewDescriptor.projectionMatrix.copy(projectionMatrix);
        viewDescriptor.viewMatrix.copy(worldMatrix);
        viewDescriptor.viewInverseMatrix.copy(viewDescriptor.viewMatrix);
        viewDescriptor.viewInverseMatrix.invert();
        viewDescriptor.viewInverseTransposeMatrix.copy(viewDescriptor.viewMatrix);
        viewDescriptor.viewInverseTransposeMatrix.transpose();
        viewDescriptor.viewInverseTransposeMatrix.invert();
    }

    void Renderer::processScene(WeakPointer<Scene> scene, 
                                std::vector<WeakPointer<Object3D>>& outObjects, 
                                std::vector<WeakPointer<Camera>>& outCameras,
                                std::vector<WeakPointer<Light>>& outLights) {
        Matrix4x4 rootTransform;
        processSceneStep(scene->getRoot(), rootTransform, outObjects, outCameras, outLights);
    }

    void Renderer::processSceneStep(WeakPointer<Object3D> object, 
                                    const Matrix4x4& curTransform, 
                                    std::vector<WeakPointer<Object3D>>& outObjects,
                                    std::vector<WeakPointer<Camera>>& outCameras, 
                                    std::vector<WeakPointer<Light>>& outLights) {
        for (GameObjectIterator<Object3D> itr = object->beginIterateChildren(); itr != object->endIterateChildren(); ++itr) {
            WeakPointer<Object3D> obj = *itr;

            Matrix4x4 nextTransform = curTransform;
            Transform& objTransform = obj->getTransform();
            nextTransform.multiply(objTransform.getLocalMatrix());
            objTransform.getWorldMatrix().copy(nextTransform);
            Matrix4x4& inverseWorld = objTransform.getInverseWorldMatrix();
            inverseWorld.copy(nextTransform);
            inverseWorld.invert();
            outObjects.push_back(obj);

            for (GameObjectIterator<Object3DComponent> compItr = obj->beginIterateComponents(); compItr != obj->endIterateComponents(); ++compItr) {
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

            this->processSceneStep(obj, nextTransform, outObjects, outCameras, outLights);
        }
    }

    Bool Renderer::isShadowCastingCapableLight(WeakPointer<Light> light) {
        LightType lightType = light->getType();
        if (lightType == LightType::Ambient || lightType == LightType::Planar) {
            return false;
        }
        return true;
    }
}
