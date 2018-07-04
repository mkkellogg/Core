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

// TODO: remove this include when light debugging is done
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
        std::vector<WeakPointer<Object3D>> objectList;
        std::vector<WeakPointer<Camera>> cameraList;
        std::vector<WeakPointer<Light>> lightList;
        this->processScene(scene, objectList, cameraList, lightList);

        for (auto camera : cameraList) {
            camera->setAspectRatioFromDimensions(this->renderSize.x, this->renderSize.y);
            ViewDescriptor viewDescriptor;
            viewDescriptor.viewMatrix.copy(camera->getOwner()->getTransform().getWorldMatrix());
            viewDescriptor.projectionMatrix.copy(camera->getProjectionMatrix());
            viewDescriptor.viewInverseTransposeMatrix.copy(viewDescriptor.viewMatrix);
            viewDescriptor.viewInverseTransposeMatrix.transpose();
            viewDescriptor.viewInverseTransposeMatrix.invert();
            render(scene, viewDescriptor, objectList, lightList);
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

    void Renderer::setRenderSize(UInt32 width, UInt32 height, Bool updateViewport) {
        this->renderSize.x = width;
        this->renderSize.y = height;
        if (updateViewport) {
            this->setViewport(0, 0, width, height);
        }
    }

    void Renderer::setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
        this->renderSize.x = width;
        this->renderSize.y = height;
        this->setViewport(hOffset, vOffset, viewPortWidth, viewPortHeight);
    }

    void Renderer::setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
        this->viewport.set(hOffset, vOffset, viewPortWidth, viewPortHeight);
    }

    Vector4u Renderer::getViewport() {
        return this->viewport;
    }
}
