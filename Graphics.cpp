#include "Graphics.h"
#include "scene/Scene.h"
#include "util/WeakPointer.h"
#include "geometry/Mesh.h"

namespace Core {

    Graphics::~Graphics() {
    }

    void Graphics::render(std::shared_ptr<Scene> scene) {
        WeakPointer<Renderer> renderer(this->getRenderer());
        if (renderer) {
            renderer->render(scene);
        }
    }
    void Graphics::setRenderSize(UInt32 width, UInt32 height, Bool updateViewport) {
        WeakPointer<Renderer> renderer(this->getRenderer());
        if (renderer) {
            renderer->setRenderSize(width, height, updateViewport);
        }
    }

    void Graphics::setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
        WeakPointer<Renderer> renderer(this->getRenderer());
        if (renderer) {
            renderer->setRenderSize(width, height, hOffset, vOffset, viewPortWidth, viewPortHeight);
        }
    }

    void Graphics::setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
        WeakPointer<Renderer> renderer(this->getRenderer());
        if (renderer) {
            renderer->setViewport(hOffset, vOffset, viewPortWidth, viewPortHeight);
        }
    }
}