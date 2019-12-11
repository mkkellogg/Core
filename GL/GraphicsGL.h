#pragma once

#include <memory>
#include <vector>

#include "../util/PersistentWeakPointer.h"
#include "../base/CoreObjectReferenceManager.h"
#include "../Graphics.h"
#include "../common/gl.h"
#include "../geometry/AttributeType.h"
#include "AttributeArrayGPUStorageGL.h"
#include "IndexBufferGL.h"
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

        virtual ~GraphicsGL();
        void init() override;
        WeakPointer<Renderer> getRenderer() override;
        void preRender() override;
        void postRender() override;
        
        void setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) override;
        Vector4u getViewport() override;
        
        WeakPointer<Texture2D> createTexture2D(const TextureAttributes& attributes) override;
        WeakPointer<CubeTexture> createCubeTexture(const TextureAttributes& attributes) override;
        
        WeakPointer<Shader> createShader(const std::string& vertex, const std::string& fragment) override;
        WeakPointer<Shader> createShader(const std::string& vertex, const std::string& geometry, const std::string& fragment) override;
        WeakPointer<Shader> createShader(const char vertex[], const char fragment[]) override;
        WeakPointer<Shader> createShader(const char vertex[], const char geometry[], const char fragment[]) override;
        void activateShader(WeakPointer<Shader> shader) override;

        void drawBoundVertexBuffer(UInt32 vertexCount) override;
        void drawBoundVertexBuffer(UInt32 vertexCount, WeakPointer<IndexBuffer> indices) override;

        ShaderManager& getShaderManager() override;

        void setBlendingEnabled(Bool enabled) override;
        void setBlendingFunction(RenderState::BlendingMethod source, RenderState::BlendingMethod dest) override;

        WeakPointer<RenderTarget2D> createRenderTarget2D(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer,
                                                     const TextureAttributes& colorTextureAttributes, 
                                                     const TextureAttributes& depthTextureAttributes, const Vector2u& size) override;
        WeakPointer<RenderTargetCube> createRenderTargetCube(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer,
                                                     const TextureAttributes& colorTextureAttributes,
                                                     const TextureAttributes& depthTextureAttributes, const Vector2u& size) override;

        void setColorWriteEnabled(Bool enabled) override;
        void setClearColor(Color color) override;
        void clearActiveRenderTarget(Bool colorBuffer, Bool depthBuffer, Bool stencilBuffer) override;
        void setDefaultRenderTargetToCurrent() override;
        WeakPointer<RenderTarget> getDefaultRenderTarget() override;
        WeakPointer<RenderTarget> getCurrentRenderTarget() override;
        void updateDefaultRenderTargetSize(Vector2u size) override;
        void updateDefaultRenderTargetViewport(Vector4u viewport) override;
        Bool activateRenderTarget(WeakPointer<RenderTarget> target) override;
        Bool activateRenderTarget2DMipLevel(UInt32 mipLevel) override;
        Bool activateCubeRenderTargetSide(CubeTextureSide side, UInt32 mipLevel) override;
        void setRenderStyle(RenderStyle style) override;

        void setDepthWriteEnabled(Bool enabled) override;
        void setDepthTestEnabled(Bool enabled) override;
        void setDepthFunction(RenderState::DepthFunction function) override;

        void setStencilTestEnabled(Bool enabled) override;
        void setStencilWriteMask(UInt32 mask) override;
        void setStencilFunction(RenderState::StencilFunction function, Int16 value, UInt16 mask) override;
        void setStencilOperation(RenderState::StencilAction sFail, RenderState::StencilAction dpFail, RenderState::StencilAction dpPass) override;

        void setFaceCullingEnabled(Bool enabled) override;
        void setCullFace(RenderState::CullFace face) override;

        void setRenderLineSize(Real size);

        void saveState() override;
        void restoreState() override;

        void lowLevelBlit(WeakPointer<RenderTarget> source, WeakPointer<RenderTarget> destination, Int16 cubeFace, Bool includeColor, Bool includeDepth) override;

        static GLint getGLDepthFunction(RenderState::DepthFunction function);
        static GLenum getGLCubeTarget(CubeTextureSide side);
        static GLuint convertAttributeType(AttributeType type);
        static GLenum getGLBlendProperty(RenderState::BlendingMethod property);
        static GLint getGLTextureFormat(TextureFormat format);
        static GLenum getGLPixelFormat(TextureFormat format);
        static GLenum getGLPixelType(TextureFormat format);
        static GLenum getGLRenderStyle(RenderStyle style);
        static GLenum getGLStencilFunction(RenderState::StencilFunction function);
        static GLenum getGLStencilAction(RenderState::StencilAction action);

    protected:

        std::shared_ptr<AttributeArrayGPUStorage> createGPUStorage(UInt32 size, UInt32 componentCount, AttributeType type, Bool normalize) override;
        std::shared_ptr<IndexBuffer> createIndexBuffer(UInt32 size) override;

    private:
        GraphicsGL(GLVersion version);
        std::shared_ptr<RendererGL> createRenderer();
        std::shared_ptr<RenderTarget2DGL> createDefaultRenderTarget();
        WeakPointer<Shader> addShader(ShaderGL* shaderPtr);
        void setupRenderState();

        GLVersion glVersion;
        std::shared_ptr<RendererGL> renderer;
        
        PersistentWeakPointer<RenderTarget2DGL> defaultRenderTarget;
        PersistentWeakPointer<RenderTarget> currentRenderTarget;
        ShaderManagerGL shaderDirectory;
        RenderStyle renderStyle;

        Vector4u _viewport;
        GLint _stateFrontFace;
        GLint _stateCullFaceMode;
        GLboolean _stateCullFaceEnabled;
        GLboolean _stateDepthTestEnabled;
        GLint _stateDepthMask;
        GLint _stateDepthFunc;
        GLboolean _stateBlendEnabled;
        GLfloat _stateLineWidth;
        GLboolean _stateLineSmoothEnabled;
        GLint _statePolygonMode[2];
    };
}