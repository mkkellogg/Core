#include "RendererGL.h"
#include "../geometry/Mesh.h"

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

    void RendererGL::render(WeakPointer<Scene> scene) {
        // TODO: Move these state calls to a place where they are not called every frame
        glClearColor(0, 0, 0, 1);
        glFrontFace(GL_CW);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LEQUAL);
        glDisable(GL_BLEND);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Renderer::render(scene);
    }

    void RendererGL::render(WeakPointer<Scene> scene, const ViewDescriptor& viewDescriptor, std::vector<WeakPointer<Object3D>>& objectList,
                            std::vector<WeakPointer<Light>>& lightList) {
        Renderer::render(scene, viewDescriptor, objectList, lightList);
    }
}
