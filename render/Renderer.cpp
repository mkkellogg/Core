#include "Renderer.h"
#include "../math/Matrix4x4.h"
#include "../util/WeakPointer.h"

namespace Core {

    Renderer::Renderer() {
    }

    Renderer::~Renderer() {
    }

    Bool Renderer::init() {
        return true;
    }

    void Renderer::processScene(std::weak_ptr<Scene> scene, std::vector<std::weak_ptr<Object3D>>& outObjects,
                                std::vector<std::weak_ptr<Camera>>& outCameras) {
        Matrix4x4 rootTransform;
        WeakPointer<Scene> scenePtr = WeakPointer<Scene>(scene);
        WeakPointer<Object3D> sceneRootPtr = WeakPointer<Object3D>(scenePtr->getRoot());
        processSceneStep(sceneRootPtr.getLockedPointer(), rootTransform, outObjects, outCameras);
    }

    void Renderer::processSceneStep(const std::weak_ptr<Object3D> object, const Matrix4x4& curTransform, std::vector<std::weak_ptr<Object3D>>& outObjects,
                                    std::vector<std::weak_ptr<Camera>>& outCameras) {
                                    WeakPointer<Object3D> objectPtr(object);
        for (auto itr = objectPtr->beginIterateChildren(); itr != objectPtr->endIterateChildren(); ++itr) {
            const std::shared_ptr<Object3D> obj = *itr;

            Matrix4x4 nextTransform = curTransform;

            Transform& objTransform = obj->getTransform();
            nextTransform.multiply(objTransform.getLocalMatrix());
            objTransform.getWorldMatrix().copy(nextTransform);
            outObjects.push_back(obj);

            for (auto compItr = obj->beginIterateComponents(); compItr != obj->endIterateComponents(); ++compItr) {
                std::weak_ptr<Object3DComponent> comp = *compItr;
                WeakPointer<Object3DComponent> compPtr(comp);

                if (compPtr) {
                  // check if this component is a camera
                  std::shared_ptr<Object3DComponent> compShared = compPtr.getLockedPointer();
                  std::shared_ptr<Camera> camShared = std::dynamic_pointer_cast<Camera>(compShared);
                  if (camShared) {
                      outCameras.push_back(camShared);
                  }
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
