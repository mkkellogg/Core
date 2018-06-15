#pragma once

#include <memory>
#include <vector>

#include "../Graphics.h"
#include "RendererGL.h"
#include "Texture2DGL.h"
#include "CubeTextureGL.h"
#include "../geometry/AttributeType.h"
#include "AttributeArrayGPUStorageGL.h"
#include "IndexBufferGL.h"

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
        
        std::weak_ptr<Texture2D> createTexture2D() override;
        std::weak_ptr<CubeTexture> createCubeTexture() override;

        std::shared_ptr<AttributeArrayGPUStorage> createGPUStorage(UInt32 size, UInt32 componentCount, AttributeType type, Bool normalize) const override;
        std::shared_ptr<IndexBuffer> createIndexBuffer(UInt32 size) const override;

    private:
        GraphicsGL(GLVersion version);
        static GLuint convertAttributeType(AttributeType type);

        GLVersion glVersion;
        std::shared_ptr<RendererGL> renderer;
        std::vector<std::shared_ptr<Texture2DGL>> textures2D;
        std::vector<std::shared_ptr<CubeTextureGL>> cubeTextures;
    };
}