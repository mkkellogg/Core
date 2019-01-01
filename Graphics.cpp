#include "Graphics.h"
#include "scene/Scene.h"
#include "geometry/Mesh.h"
#include "image/Texture2D.h"
#include "image/CubeTexture.h"
#include "material/Shader.h"
#include "render/Renderer.h"
#include "geometry/AttributeArrayGPUStorage.h"

namespace Core {

    Graphics::~Graphics() {
    }

    void Graphics::setRenderSize(UInt32 width, UInt32 height, Bool updateViewport) {
        this->updateDefaultRenderTargetSize(Vector2u(width, height));
        if (updateViewport) {
            this->setDefaultViewport(0, 0, width, height);
        }
    }

    void Graphics::setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
        this->updateDefaultRenderTargetSize(Vector2u(width, height));
        this->updateDefaultRenderTargetViewport(Vector4u(hOffset, vOffset, viewPortWidth, viewPortHeight));
    }

    void Graphics::setDefaultViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
        this->updateDefaultRenderTargetViewport(Vector4u(hOffset, vOffset, viewPortWidth, viewPortHeight));
    }
}