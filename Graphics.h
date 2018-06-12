#pragma once

#include "render/Renderer.h"

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
    };
}