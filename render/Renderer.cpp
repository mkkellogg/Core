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
#include "../material/DepthOnlyMaterial.h"
#include "../material/DistanceOnlyMaterial.h"
#include "../math/Matrix4x4.h"
#include "../math/Quaternion.h"

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
            this->distanceMaterial = Engine::instance()->createMaterial<DistanceOnlyMaterial>();
        }
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();

        std::vector<WeakPointer<Object3D>> objectList;
        std::vector<WeakPointer<Camera>> cameraList;
        std::vector<WeakPointer<Light>> lightList;
        this->processScene(scene, objectList, cameraList, lightList);

        this->renderShadowMaps(lightList, objectList);
        for (auto camera : cameraList) {
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

            orientations.push_back(forward);

            Quaternion rot;
            rot.fromAngleAxis(Math::PI / 2.0f, 0.0f, 1.0f, 0.0f);
            left = rot.rotationMatrix();
            orientations.push_back(left);

            rot = Quaternion::Identity;
            rot.fromAngleAxis(Math::PI, 0.0f, 1.0f, 0.0f);
            backward = rot.rotationMatrix();
            orientations.push_back(backward);

            rot = Quaternion::Identity;
            rot.fromAngleAxis(-Math::PI / 2.0f, 0.0f, 1.0f, 0.0f);
            right = rot.rotationMatrix();
            orientations.push_back(right);

            rot = Quaternion::Identity;
            rot.fromAngleAxis(Math::PI / 2.0f, 1.0f, 0.0f, 0.0f);
            up = rot.rotationMatrix();
            orientations.push_back(up);

            rot = Quaternion::Identity;
            rot.fromAngleAxis(-Math::PI / 2.0f, 1.0f, 0.0f, 0.0f);
            down = rot.rotationMatrix();
            orientations.push_back(down);
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
        camera->setAspectRatioFromDimensions(nextSize.x, nextSize.y);
        graphics->activateRenderTarget(nextRenderTarget);
    
        RenderTargetCube * renderTargetCube = dynamic_cast<RenderTargetCube*>(nextRenderTarget.get());
        if (renderTargetCube != nullptr) {
            for (unsigned int i = 0; i < 6; i++) {
                graphics->activateCubeRenderTargetSide((CubeTextureSide)i);
                ViewDescriptor viewDescriptor;
                viewDescriptor.overrideMaterial = overrideMaterial;
                Matrix4x4 cameraTransform = camera->getOwner()->getTransform().getWorldMatrix();
                cameraTransform.multiply(orientations[i]);
                this->getViewDescriptorForCamera(cameraTransform, camera->getProjectionMatrix(), viewDescriptor);
                viewDescriptor.overrideMaterial = overrideMaterial;
                render(viewDescriptor, objects, lights);
            }
        }
        else {
            ViewDescriptor viewDescriptor;
            viewDescriptor.overrideMaterial = overrideMaterial;
            this->getViewDescriptorForCamera(camera, viewDescriptor);
            viewDescriptor.overrideMaterial = overrideMaterial;
            render(viewDescriptor, objects, lights);
        }







        graphics->activateRenderTarget(currentRenderTarget);
        graphics->setViewport(currentViewport.x, currentViewport.y, currentViewport.z, currentViewport.w);
    }

    void Renderer::render(const ViewDescriptor& viewDescriptor, std::vector<WeakPointer<Object3D>>& objectList, std::vector<WeakPointer<Light>>& lightList) {

        for (auto object : objectList) {
            std::shared_ptr<Object3D> objectShared = object.lock();
            std::shared_ptr<BaseRenderableContainer> containerPtr = std::dynamic_pointer_cast<BaseRenderableContainer>(objectShared);
            if (containerPtr) {
                WeakPointer<BaseObjectRenderer> objectRenderer = containerPtr->getBaseRenderer();
                if (objectRenderer) {
                    objectRenderer->render(viewDescriptor, lightList);
                }
            }
        }

    }

    void Renderer::renderShadowMaps(std::vector<WeakPointer<Light>>& lights, std::vector<WeakPointer<Object3D>>& objects) {
        static PersistentWeakPointer<Camera> shadowMapCamera;
        static PersistentWeakPointer<Object3D> shadowMapCameraObject;
        if (!shadowMapCamera.isValid()) {
            shadowMapCameraObject = Engine::instance()->createObject3D();
            shadowMapCameraObject->getTransform().updateWorldMatrix();
            shadowMapCamera = Engine::instance()->createCamera(shadowMapCameraObject);
        }

        for (auto light: lights) {
            if (light->getShadowsEnabled()) {
                WeakPointer<RenderTarget> shadowMapRenderTarget = light->getShadowMap();
                WeakPointer<Object3D> lightObject = light->getOwner();
                Matrix4x4 lightTransform = lightObject->getTransform().getWorldMatrix();
                shadowMapCameraObject->getTransform().getWorldMatrix().copy(lightTransform);
                shadowMapCamera->setRenderTarget(shadowMapRenderTarget);

                this->render(shadowMapCamera, objects, lights, this->distanceMaterial);
            }
        }
    }
    
    void Renderer::getViewDescriptorForCamera(WeakPointer<Camera> camera, ViewDescriptor& viewDescriptor) {
        this->getViewDescriptorForCamera(camera->getOwner()->getTransform().getWorldMatrix(), camera->getProjectionMatrix(), viewDescriptor);
    }

    void Renderer::getViewDescriptorForCamera(const Matrix4x4& worldMatrix, const Matrix4x4& projectionMatrix, ViewDescriptor& viewDescriptor) {
        viewDescriptor.viewMatrix.copy(worldMatrix);
        viewDescriptor.projectionMatrix.copy(projectionMatrix);
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
}
