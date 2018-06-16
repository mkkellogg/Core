#include "Graphics.h"
#include "scene/Scene.h"
#include "geometry/Mesh.h"

namespace Core {

    Graphics::~Graphics() {
    }

    void Graphics::render(std::shared_ptr<Scene> scene) {
        if (this->getRenderer()) {
            this->getRenderer()->render(scene);
        }
    }
    void Graphics::setRenderSize(UInt32 width, UInt32 height, Bool updateViewport) {
        if (this->getRenderer()) {
            this->getRenderer()->setRenderSize(width, height, updateViewport);
        }
    }

    void Graphics::setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
        if (this->getRenderer()) {
            this->getRenderer()->setRenderSize(width, height, hOffset, vOffset, viewPortWidth, viewPortHeight);
        }
    }

    void Graphics::setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
        if (this->getRenderer()) {
            this->getRenderer()->setViewport(hOffset, vOffset, viewPortWidth, viewPortHeight);
        }
    }
}