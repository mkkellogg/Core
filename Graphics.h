#pragma once

#include <memory>
#include <vector>

#include "util/WeakPointer.h"
#include "image/TextureAttr.h"
#include "geometry/AttributeType.h"
#include "render/RenderState.h"
#include "geometry/Vector2.h"
#include "geometry/Vector4.h"

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
    
    class Graphics {
    public:
        virtual ~Graphics();
        virtual void init() = 0;

        virtual WeakPointer<Renderer> getRenderer() = 0;
        void render(std::shared_ptr<Scene> scene);
        void setRenderSize(UInt32 width, UInt32 height, Bool updateViewport = true);
        void setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);
        void setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);

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

        virtual WeakPointer<RenderTarget> createRenderTarget(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer,
                                                             const TextureAttributes& colorTextureAttributes, Vector2u size) = 0;
        virtual WeakPointer<RenderTarget> getDefaultRenderTarget() = 0;
        virtual WeakPointer<RenderTarget> getCurrentRenderTarget() = 0;
        virtual void updateDefaultRenderTargetSize(Vector2u size) = 0;
        virtual void updateDefaultRenderTargetViewport(Vector4u viewport) = 0;
        virtual Bool activateRenderTarget(WeakPointer<RenderTarget> target) = 0;

    };
}