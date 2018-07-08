#pragma once

#include "../common/gl.h"
#include "../render/Renderer.h"

namespace Core {

    // forward declaration
    class GraphicsGL;
    class Scene;
    class Camera;
    class Object3D;
    class Light;

    class RendererGL final : public Renderer {
        friend class GraphicsGL;

    public:
        ~RendererGL();
        Bool init() override;
  
    private:
        RendererGL();
    };
}
