#include <vector>

#include "Renderer.h"
#include "../math/Matrix4x4.h"
#include "../render/BaseRenderableContainer.h"
#include "../render/MeshRenderer.h"
#include "../render/RenderableContainer.h"
#include "../scene/Scene.h"
#include "Camera.h"

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
        this->processScene(scene, objectList, cameraList);
        for (auto camera : cameraList) {
            camera->setAspectRatioFromDimensions(this->renderSize.x, this->renderSize.y);
            render(scene, camera, objectList);
        }
    }

     void Renderer::render(WeakPointer<Scene> scene, WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objectList) {
        for (auto object : objectList) {
            std::shared_ptr<Object3D> objectShared = object.lock();
            std::shared_ptr<BaseRenderableContainer> containerPtr = std::dynamic_pointer_cast<BaseRenderableContainer>(objectShared);
            if (containerPtr) {
                WeakPointer<BaseObjectRenderer> objectRenderer = containerPtr->getBaseRenderer();
                if (objectRenderer) {
                    objectRenderer->render(camera);
                }
            }
        }
    }

    void Renderer::processScene(WeakPointer<Scene> scene, std::vector<WeakPointer<Object3D>>& outObjects, std::vector<WeakPointer<Camera>>& outCameras) {
        Matrix4x4 rootTransform;
        processSceneStep(scene->getRoot(), rootTransform, outObjects, outCameras);
    }

    void Renderer::processSceneStep(WeakPointer<Object3D> object, const Matrix4x4& curTransform, std::vector<WeakPointer<Object3D>>& outObjects,
                                    std::vector<WeakPointer<Camera>>& outCameras) {
        for (GameObjectIterator<Object3D> itr = object->beginIterateChildren(); itr != object->endIterateChildren(); ++itr) {
            WeakPointer<Object3D> obj = *itr;

            Matrix4x4 nextTransform = curTransform;
            Transform& objTransform = obj->getTransform();
            nextTransform.multiply(objTransform.getLocalMatrix());
            objTransform.getWorldMatrix().copy(nextTransform);
            outObjects.push_back(obj);

            for (GameObjectIterator<Object3DComponent> compItr = obj->beginIterateComponents(); compItr != obj->endIterateComponents(); ++compItr) {
                // check if this component is a camera
                std::shared_ptr<Object3DComponent> compShared = (*compItr).lock();
                std::shared_ptr<Camera> camShared = std::dynamic_pointer_cast<Camera>(compShared);
                if (camShared) {
                    outCameras.push_back(camShared);
                }
            }

            this->processSceneStep(obj, nextTransform, outObjects, outCameras);
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
