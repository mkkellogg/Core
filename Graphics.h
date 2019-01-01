#pragma once

#include <memory>
#include <vector>

#include "util/WeakPointer.h"
#include "image/TextureAttr.h"
#include "geometry/AttributeType.h"
#include "render/RenderState.h"
#include "render/RenderBuffer.h"
#include "render/RenderStyle.h"
#include "geometry/Vector2.h"
#include "geometry/Vector4.h"
#include "color/Color.h"

namespace Core {

    // forward declarations
    class Mesh;
    class Texture2D;
    class CubeTexture;
    class Shader;
    class AttributeArrayGPUStorage;
    class IndexBuffer;
    class Renderer;
    class Scene;
    class ShaderManager;
    class RenderTarget;
    class RenderTarget2D;
    class RenderTargetCube;
    
    class Graphics {
    public:
        virtual ~Graphics();
        virtual void init() = 0;

        virtual WeakPointer<Renderer> getRenderer() = 0;
        virtual void preRender() = 0;
        virtual void postRender() = 0;
        void setRenderSize(UInt32 width, UInt32 height, Bool updateViewport = true);
        void setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);
        virtual void setDefaultViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);
        virtual void setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) = 0;
        virtual Vector4u getViewport() = 0;

        virtual WeakPointer<Texture2D> createTexture2D(const TextureAttributes& attributes) = 0;
        virtual WeakPointer<CubeTexture> createCubeTexture(const TextureAttributes& attributes) = 0;
        virtual void destroyTexture2D(WeakPointer<Texture2D> texture) = 0;
        virtual void destroyCubeTexture(WeakPointer<CubeTexture> texture) = 0;

        virtual WeakPointer<Shader> createShader(const std::string& vertex, const std::string& fragment) = 0;
        virtual WeakPointer<Shader> createShader(const char vertex[], const char fragment[]) = 0;
        virtual void activateShader(WeakPointer<Shader> shader) = 0;
    
        virtual std::shared_ptr<AttributeArrayGPUStorage> createGPUStorage(UInt32 size, UInt32 componentCount, AttributeType type, Bool normalize) const = 0;
        virtual std::shared_ptr<IndexBuffer> createIndexBuffer(UInt32 size) const = 0;

        virtual void drawBoundVertexBuffer(UInt32 vertexCount) = 0;
        virtual void drawBoundVertexBuffer(UInt32 vertexCount, WeakPointer<IndexBuffer> indices) = 0;

        virtual ShaderManager& getShaderManager() = 0;

        virtual void setBlendingEnabled(Bool enabled) = 0;
        virtual void setBlendingFunction(RenderState::BlendingMethod source, RenderState::BlendingMethod dest) = 0;

        virtual WeakPointer<RenderTarget2D> createRenderTarget2D(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer,
                                                                 const TextureAttributes& colorTextureAttributes,
                                                                 const TextureAttributes& depthTextureAttributes, Vector2u size) = 0;
        virtual WeakPointer<RenderTargetCube> createRenderTargetCube(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer,
                                                                     const TextureAttributes& colorTextureAttributes,
                                                                     const TextureAttributes& depthTextureAttributes, Vector2u size) = 0;
                                                                     
        virtual void setClearColor(Color color) = 0;
        virtual void clearActiveRenderTarget(Bool colorBuffer, Bool depthBuffer, Bool stencilBuffer) = 0;
        virtual void setDefaultRenderTargetToCurrent() = 0;
        virtual WeakPointer<RenderTarget> getDefaultRenderTarget() = 0;
        virtual WeakPointer<RenderTarget> getCurrentRenderTarget() = 0;
        virtual void updateDefaultRenderTargetSize(Vector2u size) = 0;
        virtual void updateDefaultRenderTargetViewport(Vector4u viewport) = 0;
        virtual Bool activateRenderTarget(WeakPointer<RenderTarget> target) = 0;
        virtual Bool activateCubeRenderTargetSide(CubeTextureSide side) = 0;
        virtual void setRenderingToBufferEnabled(RenderBufferType type, Bool enabled) = 0;
        virtual void setRenderStyle(RenderStyle style) = 0;
        virtual void setDepthTestEnabled(Bool enabled) = 0;

        virtual void saveState() = 0;
        virtual void restoreState() = 0;
    };
}