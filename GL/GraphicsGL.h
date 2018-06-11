#pragma once

#include <memory>

#include "../Graphics.h"
#include "RendererGL.h"

namespace Core {

    // forward declarations
    class Engine;

    class GraphicsGL final: public Graphics {
        friend class Engine;

    public:
        enum class GLVersion {
            Two = 2,
            Three = 3,
        };

        ~GraphicsGL();
        void init() override;
        std::weak_ptr<Renderer> getRenderer() override;
        
    private:
        GraphicsGL(GLVersion version);
        GLVersion glVersion;
        std::shared_ptr<RendererGL> renderer;
    };

}