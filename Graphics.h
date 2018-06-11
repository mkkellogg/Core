#pragma once

#include "render/Renderer.h"

namespace Core {

    class Graphics {
    public:
        virtual ~Graphics();
        virtual void init() = 0;
        virtual std::weak_ptr<Renderer> getRenderer() = 0;
    };

}