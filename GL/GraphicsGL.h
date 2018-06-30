#pragma once

#include <memory>
#include <vector>

#include "../util/WeakPointer.h"
#include "../Graphics.h"
#include "../common/gl.h"
#include "../geometry/AttributeType.h"
#include "ShaderManagerGL.h"

namespace Core {

    // forward declarations
    class Engine;
    class RendererGL;
    class Texture2DGL;
    class ShaderGL;
    class CubeTextureGL;

    class GraphicsGL final : public Graphics {
        friend class Engine;

    public:
        enum class GLVersion {
            Two = 2,
            Three = 3,
        };

        ~GraphicsGL();
        void init() override;
        WeakPointer<Renderer> getRenderer() override;
        
        WeakPointer<Texture2D> createTexture2D(const TextureAttributes& attributes) override;
        WeakPointer<CubeTexture> createCubeTexture(const TextureAttributes& attributes) override;
        
        WeakPointer<Shader> createShader(const std::string& vertex, const std::string& fragment) override;
        WeakPointer<Shader> createShader(const char vertex[], const char fragment[]) override;
        void activateShader(WeakPointer<Shader> shader) override;

        std::shared_ptr<AttributeArrayGPUStorage> createGPUStorage(UInt32 size, UInt32 componentCount, AttributeType type, Bool normalize) const override;
        std::shared_ptr<IndexBuffer> createIndexBuffer(UInt32 size) const override;

        void drawBoundVertexBuffer(UInt32 vertexCount) override;
        void drawBoundVertexBuffer(UInt32 vertexCount, WeakPointer<IndexBuffer> indices) override;

        ShaderManager& getShaderManager() override;

        void setBlendingEnabled(Bool enabled) override;
        void setBlendingFunction(RenderState::BlendingMethod source, RenderState::BlendingMethod dest) override;

    private:
        GraphicsGL(GLVersion version);
        static GLuint convertAttributeType(AttributeType type);
        static GLenum getGLBlendProperty(RenderState::BlendingMethod property);

        GLVersion glVersion;
        std::shared_ptr<RendererGL> renderer;
        std::vector<std::shared_ptr<Texture2DGL>> textures2D;
        std::vector<std::shared_ptr<CubeTextureGL>> cubeTextures;
        std::vector<std::shared_ptr<ShaderGL>> shaders;
        ShaderManagerGL shaderDirectory;
    };
}