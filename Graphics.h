#pragma once

#include <memory>

#include "render/Renderer.h"
#include "image/Texture2D.h"
#include "image/CubeTexture.h"
#include "geometry/AttributeArray.h"
#include "geometry/AttributeType.h"
#include "geometry/AttributeArrayGPUStorage.h"

namespace Core {

    class Graphics {
    public:
        virtual ~Graphics();
        virtual void init() = 0;

        virtual std::weak_ptr<Renderer> getRenderer() = 0;
        void render(std::shared_ptr<Scene> scene);
        void setRenderSize(UInt32 width, UInt32 height, Bool updateViewport = true);
        void setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);
        void setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);

        virtual std::weak_ptr<Texture2D> createTexture2D() = 0;
        virtual std::weak_ptr<CubeTexture> createCubeTexture() = 0;
        virtual std::weak_ptr<Mesh> createMesh(UInt32 size, Bool indexed) = 0;
    
        virtual std::shared_ptr<AttributeArrayGPUStorage> createGPUStorage(UInt32 size, UInt32 componentCount, AttributeType type, Bool normalize) const = 0;

    };
}