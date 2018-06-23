#pragma once

#include <memory>

#include "util/WeakPointer.h"
#include "render/Renderer.h"
#include "image/Texture2D.h"
#include "image/CubeTexture.h"
#include "image/TextureAttr.h"
#include "material/Shader.h"
#include "geometry/IndexBuffer.h"
#include "geometry/AttributeArray.h"
#include "geometry/AttributeType.h"
#include "geometry/AttributeArrayGPUStorage.h"

namespace Core {

    // forward declarations
    class Mesh;
    
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

        virtual WeakPointer<Shader> createShader(const std::string& vertex, const std::string& fragment) = 0;
        virtual WeakPointer<Shader> createShader(const char vertex[], const char fragment[]) = 0;
        virtual void activateShader(WeakPointer<Shader> shader) = 0;
    
        virtual std::shared_ptr<AttributeArrayGPUStorage> createGPUStorage(UInt32 size, UInt32 componentCount, AttributeType type, Bool normalize) const = 0;
        virtual std::shared_ptr<IndexBuffer> createIndexBuffer(UInt32 size) const = 0;

        virtual void drawBoundVertexBuffer(UInt32 vertexCount) = 0;
        virtual void drawBoundVertexBuffer(UInt32 vertexCount, WeakPointer<IndexBuffer> indices) = 0;

    protected:
        std::vector<std::shared_ptr<Mesh>> meshes;

    };
}