#include "RendererGL.h"
#include "../render/BaseRenderableContainer.h"
#include "../render/MeshRenderer.h"
#include "../render/RenderableContainer.h"

#if !defined(DYNAMIC_ES3)
static GLboolean gl3stubInit() {
    return GL_TRUE;
}
#endif

namespace Core {

    RendererGL::RendererGL() {
    }

    Bool RendererGL::init() {
        Renderer::init();
        return true;
    }

    RendererGL::~RendererGL() {
    }

    RendererGL* RendererGL::createRenderer() {
        RendererGL* renderer = new RendererGL;
        if (!renderer->init()) {
            delete renderer;
            return nullptr;
        }
        return renderer;
    }

    void RendererGL::setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
        Renderer::setViewport(hOffset, vOffset, viewPortWidth, viewPortHeight);
        glViewport(hOffset, vOffset, viewPortWidth, viewPortHeight);
    }

    void RendererGL::render(WeakPointer<Scene> scene) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        std::vector<WeakPointer<Object3D>> objectList;
        std::vector<WeakPointer<Camera>> cameraList;
        this->processScene(scene, objectList, cameraList);
        for (auto camera : cameraList) {
            camera->setAspectRatioFromDimensions(this->renderSize.x, this->renderSize.y);
            render(scene, camera, objectList);
        }
    }

    void RendererGL::render(WeakPointer<Scene> scene, WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objectList) {
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
}
