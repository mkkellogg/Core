#pragma once

#include <memory>
#include <vector>

#include "../Graphics.h"
#include "RendererGL.h"
#include "Texture2DGL.h"

namespace Core {

    // forward declarations
    class Engine;

    class GraphicsGL final : public Graphics {
        friend class Engine;

    public:
        enum class GLVersion {
            Two = 2,
            Three = 3,
        };

        ~GraphicsGL();
        void init() override;
        std::weak_ptr<Renderer> getRenderer() override;
        std::weak_ptr<Texture> createTexture() override;

    private:
        GraphicsGL(GLVersion version);
        GLVersion glVersion;
        std::shared_ptr<RendererGL> renderer;
        std::vector<std::shared_ptr<Texture2DGL>> textures;
    };
}