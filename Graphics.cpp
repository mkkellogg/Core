#include "Graphics.h"
#include "util/ValidWeakPointer.h"
#include "scene/Scene.h"

namespace Core {

    Graphics::~Graphics() {

    }

    void Graphics::render(std::shared_ptr<Scene> scene) {
        ValidWeakPointer<Renderer> renderer(this->getRenderer());
        if(renderer.isInitialized()) {
            renderer->render(scene);
        }
    }

    void Graphics::setRenderSize(UInt32 width, UInt32 height, Bool updateViewport) {
        ValidWeakPointer<Renderer> renderer(this->getRenderer());
        if (renderer.isInitialized()) {
            renderer->setRenderSize(width, height, updateViewport);
        }
    }

    void Graphics::setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32  viewPortHeight) {
        ValidWeakPointer<Renderer> renderer(this->getRenderer());
        if (renderer.isInitialized()) {
            renderer->setRenderSize(width, height, hOffset, vOffset, viewPortWidth, viewPortHeight);
        }
    }

    void Graphics::setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
        ValidWeakPointer<Renderer> renderer(this->getRenderer());
        if (renderer.isInitialized()) {
            renderer->setViewport(hOffset, vOffset, viewPortWidth, viewPortHeight);
        }
    }
}