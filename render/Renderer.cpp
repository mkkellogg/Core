#include "Renderer.h"
#include "../math/Matrix4x4.h"

namespace Core {

    Renderer::Renderer() {
    }

    Renderer::~Renderer() {
    }

    Bool Renderer::init() {
        return true;
    }

    void Renderer::processScene(WeakPointer<Scene> scene, std::vector<WeakPointer<Object3D>>& outObjects, std::vector<WeakPointer<Camera>>& outCameras) {
        Matrix4x4 rootTransform;
        processSceneStep(scene->getRoot(), rootTransform, outObjects, outCameras);
    }

    void Renderer::processSceneStep(WeakPointer<Object3D> object, const Matrix4x4& curTransform, std::vector<WeakPointer<Object3D>>& outObjects,
                                    std::vector<WeakPointer<Camera>>& outCameras) {
        for (Object3D::ChildIterator itr = object->beginIterateChildren(); itr != object->endIterateChildren(); ++itr) {
            WeakPointer<Object3D> obj = *itr;

            Matrix4x4 nextTransform = curTransform;
            Transform& objTransform = obj->getTransform();
            nextTransform.multiply(objTransform.getLocalMatrix());
            objTransform.getWorldMatrix().copy(nextTransform);
            outObjects.push_back(obj);

            for (auto compItr = obj->beginIterateComponents(); compItr != obj->endIterateComponents(); ++compItr) {
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
