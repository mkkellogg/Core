#pragma once

#include <memory>

namespace Core {

    // forward declarations
    class Engine;

    class GraphicsGL final {
        friend class Engine;

    public:
        enum class GLVersion {
            Two = 2,
            Three = 3,
        };

        ~GraphicsGL() override;
        void init() override;
        std::weak_ptr<Renderer> getRenderer() override;
    private:
        GraphicsGL();
        GLVersion glVersion;
        std::shared_ptr<RendererGL> renderer;
    };

}