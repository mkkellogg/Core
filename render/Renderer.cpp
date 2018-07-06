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

namespace Core {

    Renderer::Renderer() {
    }

    Renderer::~Renderer() {
    }

    Bool Renderer::init() {
        return true;
    }

    void Renderer::render(WeakPointer<Scene> scene) {
        std::vector<WeakPointer<Object3D>> objectList;
        std::vector<WeakPointer<Camera>> cameraList;
        std::vector<WeakPointer<Light>> lightList;
        this->processScene(scene, objectList, cameraList, lightList);
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();

        for (auto camera : cameraList) {
            WeakPointer<RenderTarget> currentRenderTarget = graphics->getCurrentRenderTarget();
           // Vector2u currentRenderSize = this->getRenderSize();
            Vector4u currentViewport = currentRenderTarget->getViewport();
            
            WeakPointer<RenderTarget> nextRenderTarget = camera->getRenderTarget();
            if (!nextRenderTarget.isValid()) {
                nextRenderTarget = graphics->getDefaultRenderTarget();
            }
            graphics->activateRenderTarget(nextRenderTarget);

            Vector2u nextSize = nextRenderTarget->getSize();
            Vector4u nextViewport = nextRenderTarget->getViewport();
            graphics->setViewport(nextViewport.x, nextViewport.y, nextViewport.z, nextViewport.w);
            camera->setAspectRatioFromDimensions(nextSize.x, nextSize.y);

            ViewDescriptor viewDescriptor;
            this->getViewDescriptorForCamera(camera, viewDescriptor);
            render(scene, viewDescriptor, objectList, lightList);

            graphics->activateRenderTarget(currentRenderTarget);
            //this->setRenderSize(currentRenderSize.x, currentRenderSize.y);
            graphics->setViewport(currentViewport.x, currentViewport.y, currentViewport.z, currentViewport.w);
        }
    }

    void Renderer::render(WeakPointer<Scene> scene, 
                          const ViewDescriptor& viewDescriptor, 
                          std::vector<WeakPointer<Object3D>>& objectList,
                          std::vector<WeakPointer<Light>>& lightList) {

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

    void Renderer::getViewDescriptorForCamera(WeakPointer<Camera> camera, ViewDescriptor& viewDescriptor) {
        viewDescriptor.viewMatrix.copy(camera->getOwner()->getTransform().getWorldMatrix());
        viewDescriptor.projectionMatrix.copy(camera->getProjectionMatrix());
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
