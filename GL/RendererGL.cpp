#include "RendererGL.h"

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
        Renderer::render(scene);
    }

    void RendererGL::render(WeakPointer<Scene> scene, WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objectList) {
        Renderer::render(scene, camera, objectList);
    }
}
