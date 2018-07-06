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
        void render(WeakPointer<Scene> scene) override;
        void render(WeakPointer<Scene> scene, 
                    const ViewDescriptor& viewDescriptor, 
                    std::vector<WeakPointer<Object3D>>& objectList, 
                    std::vector<WeakPointer<Light>>& lightList) override;

    private:
        RendererGL();
    };
}
