#pragma once

#include <memory>
#include <vector>

#include "../util/PersistentWeakPointer.h"
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
    class RenderTargetGL;
    class RenderTarget2DGL;
    class RenderTargetCubeGL;

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
        void preRender() override;
        void postRender() override;
        
        void setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) override;
        Vector4u getViewport() override;
        
        WeakPointer<Texture2D> createTexture2D(const TextureAttributes& attributes) override;
        WeakPointer<CubeTexture> createCubeTexture(const TextureAttributes& attributes) override;
        void destroyTexture2D(WeakPointer<Texture2D> texture) override;
        void destroyCubeTexture(WeakPointer<CubeTexture> texture) override;
        
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

        WeakPointer<RenderTarget2D> createRenderTarget2D(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer,
                                                     const TextureAttributes& colorTextureAttributes, 
                                                     const TextureAttributes& depthTextureAttributes, Vector2u size) override;
        WeakPointer<RenderTargetCube> createRenderTargetCube(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer,
                                                     const TextureAttributes& colorTextureAttributes,
                                                     const TextureAttributes& depthTextureAttributes, Vector2u size) override;

        void setClearColor(Color color) override;
        void clearActiveRenderTarget(Bool colorBuffer, Bool depthBuffer, Bool stencilBuffer) override;
        WeakPointer<RenderTarget> getDefaultRenderTarget() override;
        WeakPointer<RenderTarget> getCurrentRenderTarget() override;
        void updateDefaultRenderTargetSize(Vector2u size) override;
        void updateDefaultRenderTargetViewport(Vector4u viewport) override;
        Bool activateRenderTarget(WeakPointer<RenderTarget> target) override;
        Bool activateCubeRenderTargetSide(CubeTextureSide side) override;
        void setRenderingToBufferEnabled(RenderBufferType type, Bool enabled) override;
        void setRenderStyle(RenderStyle style) override;

        static GLenum getGLCubeTarget(CubeTextureSide side);
        static GLuint convertAttributeType(AttributeType type);
        static GLenum getGLBlendProperty(RenderState::BlendingMethod property);
        static GLint getGLTextureFormat(TextureFormat format);
        static GLenum getGLPixelFormat(TextureFormat format);
        static GLenum getGLPixelType(TextureFormat format);
        static GLuint getGLRenderStyle(RenderStyle style);

    private:
        GraphicsGL(GLVersion version);
        std::shared_ptr<RendererGL> createRenderer();
        std::shared_ptr<RenderTarget2DGL> createDefaultRenderTarget();

        GLVersion glVersion;
        std::shared_ptr<RendererGL> renderer;
        std::vector<std::shared_ptr<Texture2DGL>> textures2D;
        std::vector<std::shared_ptr<CubeTextureGL>> cubeTextures;
        std::vector<std::shared_ptr<ShaderGL>> shaders;
        PersistentWeakPointer<RenderTarget2DGL> defaultRenderTarget;
        std::vector<std::shared_ptr<RenderTarget2DGL>> renderTarget2Ds;
        std::vector<std::shared_ptr<RenderTargetCubeGL>> renderTargetCubes;
        PersistentWeakPointer<RenderTarget> currentRenderTarget;
        ShaderManagerGL shaderDirectory;
        Vector4u viewport;
        RenderStyle renderStyle;

        GLint _stateFrontFace;
        GLint _stateCullFaceMode;
        GLboolean _stateCullFaceEnabled;
        GLboolean _stateDepthTestEnabled;
        GLint _stateDepthMask;
        GLint _stateDepthFunc;
        GLboolean _stateBlendEnabled;
    };
}